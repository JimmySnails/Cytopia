#ifndef GAME_CLOCK_HXX_
#define GAME_CLOCK_HXX_

#include <queue>
#include <chrono>
#include <mutex>
#include <functional>
#include "../GameService.hxx"

using namespace std::chrono_literals;

/**
 * @brief Game clock service. Implement two timer one real time timer and other game time timer.
 * Both timers provide possibility to add task which will be triggered after delay time run out.
 * Game timer represent timer running in game time.
 * The game time can be scaled providing possibility to speed up or slow down game.
 */
class GameClock : public GameService
{
private:
  using Clock = std::chrono::high_resolution_clock;

  /**
  * @brief Duration of default game timer tick in ms.
  */
  const unsigned int DefaultGameTickDuration = 2000;

public:
  using TimePoint = std::chrono::time_point<Clock>;
  using ClockCbk = std::function<void(void)>;
  using ClockTaskHndl = unsigned long;
  using GameClockTime = unsigned long;

  /**
  * @brief Represent 1 minute of game time.
  */
  static const GameClockTime GameMinut = 1;

  /**
  * @brief Represent 1 hour of game time.
  */
  static const GameClockTime GameHour = 60 * GameMinut;

  /**
  * @brief Represent 1 day of game time.
  */
  static const GameClockTime GameDay = 24 * GameHour;

  /**
  * @brief Invalid clock handle. Represent error value during clock task creation.
  */
  static constexpr ClockTaskHndl ClockTaskHndlInvalid = ClockTaskHndl(0);

  /**
  * @brief Constructor.
  */
  GameClock(GameService::ServiceTuple &);

  /**
  * @brief This function provide the tick for both clocks.
  * It must be called frequently. Call frequency determines clock precision.
  */
  void tick(void);

  /**
  * @brief Add new real time clock task.
  * @param cbk Callback function to be called after delay time is passed.
  * @param delay Delay in chrono literals (e.g. 1h, 2min, 3s ...), callback function will be called after delay timer is passed.
  * @param period Repeat period in chrono literals (e.g. 1h, 2min, 3s ...) the timer will be reset again with new delay time in amount of period time.
  * @return Real time clock task handle. Can be used to remove clock and verify whether it is started correctly.
  *         In case of failure #ClockTaskHndlInvalid will be return.
  */
  template <typename DelayType, typename PeriodType = TimePoint>
  GameClock::ClockTaskHndl addRealTimeClockTask(ClockCbk cbk, DelayType delay, PeriodType period = TimePointZero);

  /**
  * @brief Add new game time clock task.
  * @param cbk Callback function to be called after delay time is passed.
  * @param delay Delay in game timer ticks. Use provided values GameDay, GameHour, GameMinut and scale it as necessary.
  *        Callback function will be called after delay timer is passed.
  * @param period Repeat period in game timer ticks. Use provided values GameDay, GameHour, GameMinut and scale it as necessary.
  *        The timer will be reset again with new delay ticks in amount of period ticks.
  * @return Game time clock task handle. Can be used to remove clock and verify whether it is started correctly.
  *         In case of failure #ClockTaskHndlInvalid will be return.
  */
  GameClock::ClockTaskHndl addGameTimeClockTask(ClockCbk cbk, GameClockTime delay, GameClockTime period = 0U);

  /**
  * @brief Set game clock speed.
  * @param speedFactor Game clock scale factor.
  * E.g. to run game clock 4 time faster provide 4.0f.
  */
  void setGameClockSpeed(float speedFactor);

  /**
  * @brief Remove all real time and game time clocks.
  */
  void clear(void);

  /**
  * @brief Remove real/game time clock.
  *        After it is removed successfully it is guarantied it will not trigger callback 
  * @param hndl Handle of clock which should be removed.
  * @return true in case clock is successfully removed, otherwise false.
  */
  bool removeClockTask(ClockTaskHndl hndl);

private:
  static constexpr TimePoint TimePointZero = TimePoint{0s};

  /**
  * Structure represent real time clock task.
  */
  struct RealTimeClockTask
  {
    std::function<void(void)> callback;
    TimePoint m_waketime;
    TimePoint m_period;
    ClockTaskHndl hndl;
    explicit RealTimeClockTask(ClockCbk cbk, TimePoint delay, TimePoint period, ClockTaskHndl hndl)
        : callback(cbk), m_waketime(delay), m_period(period), hndl(hndl)
    {
    }

    inline friend bool operator>(const RealTimeClockTask &rt1, const RealTimeClockTask &rt2)
    {
      return rt1.m_waketime > rt2.m_waketime;
    }
  };

  /**
  * Structure represent game time clock task.
  */
  struct GameTimeClockTask
  {
    std::function<void(void)> callback;
    GameClockTime m_waketime;
    GameClockTime m_period;
    ClockTaskHndl hndl;
    explicit GameTimeClockTask(ClockCbk cbk, GameClockTime delay, GameClockTime period, ClockTaskHndl hndl)
        : callback(cbk), m_waketime(delay), m_period(period), hndl(hndl)
    {
    }

    inline friend bool operator>(const GameTimeClockTask &gt1, const GameTimeClockTask &gt2)
    {
      return gt1.m_waketime > gt2.m_waketime;
    }
  };

  /**
  * @brief Wrap priority queue class to extend functionality.
  */
  template <typename Task> class RemovablePriorityQueue : public std::priority_queue<Task, std::deque<Task>, std::greater<Task>>
  {
  public:
    /**
    * @brief Remove task from queue.
    * @param hndl Handle of the task to be removed.
    */
    bool remove(ClockTaskHndl hndl);

    /**
    * @brief Remove all tasks from queue.
    */
    void clear(void);
  };

  RemovablePriorityQueue<RealTimeClockTask> m_realTimeTasks;
  RemovablePriorityQueue<GameTimeClockTask> m_gameTimeTasks;
  std::mutex m_lock;
  // Provide way to return unique handle for each task.
  ClockTaskHndl m_unique_handle = 0U;
  // Current number of the game ticks.
  GameClockTime m_gameTicks = 0U;
  // Last time of the game tick.
  TimePoint m_lastGameTickTime;
  // The current game tick duration on milliseconds.
  Clock::duration m_gameTickDuration = std::chrono::milliseconds(DefaultGameTickDuration);
};

#include "GameClock.inl.hxx"

#endif
