#ifndef MOUSE_EVENTS_HXX
#define MOUSE_EVENTS_HXX

#include "../util/Meta.hxx"

union SDL_Event;

/**
 * @brief A change in the mouse (or device)'s position
 */
struct MousePositionEvent
{
  int xPosition;
  int yPosition;
  int xVelocity;
  int yVelocity;
};

/**
 * @brief A click interaction
 */
struct ClickEvent
{
  int32_t xPosition;
  int32_t yPosition;
  /**
   * @brief One-hot encoding for the state of the mouse or device
   * @details Exactly one of {Left, Right, Middle} must be set 
   *          and exactly one of {Pressed, Released} must be set
   */
  static constexpr uint8_t Left = 0b1;
  static constexpr uint8_t Right = 0b10;
  static constexpr uint8_t Middle = 0b100;
  static constexpr uint8_t Pressed = 0b1000;
  static constexpr uint8_t Released = 0b10000;
  uint8_t state;
};

/**
 * @brief A scrolling interaction
 */
struct ScrollEvent
{
  int32_t xDisplacement;
  int32_t yDisplacement;
};

using MouseEvents = TypeList<
  MousePositionEvent,
  ClickEvent,
  ScrollEvent
>;

#endif // MOUSE_EVENTS_HXX
