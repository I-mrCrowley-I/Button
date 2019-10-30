#include "Button.h"

Button::Button()
{
  init();
  setDebounce(DEFAULT_TIME_DEBOUNCE);
}

Button::Button(uint8_t pin)
{
  init();
  setPin(pin);
  setDebounce(DEFAULT_TIME_DEBOUNCE);
}

Button::Button(uint8_t pin, uint16_t debounce)
{
  init();
  setPin(pin);
  setDebounce(debounce);
}

Button::Button(uint8_t pin, uint16_t debounce, handler oneClick)
{
  init();
  setPin(pin);
  setDebounce(debounce);
  setHandler(oneClick);
}

Button::~Button()
{
}

void Button::init()
{
  previousDebounceTime =
      previousDoubleClickTime =
          previousHoldTime = millis();

  timeHold = DEFAULT_TIME_HOLD;
  timeDoubleClick = DEFAULT_TIME_DOUBLE_CLICK;

  flag.PRESSED = false;
  flag.POLARITY = NEGATIVE;
  flag.WAS_PRESSED = false;
  flag.WAS_RELEASED = false;
  flag.WAS_HOLD = false;
  flag.WAS_DOUBLE_CLICK = false;
}

bool Button::readPin()
{
  return digitalRead(pin);
}

void Button::setPin(uint8_t pin)
{
  this->pin = pin;
  pinMode(this->pin, INPUT_PULLUP);
}

void Button::setPolarity(bool polarity)
{
  if (flag.POLARITY != polarity)
  {
    if (polarity == NEGATIVE)
    {
      flag.POLARITY = NEGATIVE;
      digitalWrite(pin, HIGH);
    }
    else
    {
      flag.POLARITY = POSITIVE;
      digitalWrite(pin, LOW);
    }
  }
}

void Button::loop()
{
  bool state;

  if (flag.POLARITY == POSITIVE)
    state = !readPin();
  else
    state = readPin();

  if (!state && !flag.PRESSED &&
      millis() - previousDebounceTime > timeDebounce)
  {
    flag.PRESSED = true;
    if (millis() - previousDoubleClickTime < timeDoubleClick)
    {
      flag.WAS_DOUBLE_CLICK = true;
      if (doubleClick)
      {
        doubleClick();
        flag.WAS_DOUBLE_CLICK = false;
      }
    }
    else
    {
      flag.WAS_PRESSED = true;
      if (oneClick)
      {
        oneClick();
        flag.WAS_PRESSED = false;
      }
    }
    previousHoldTime = millis();
  }
  else if (millis() - previousHoldTime > timeHold &&
           flag.PRESSED)
  {
    flag.WAS_HOLD = true;
    if (hold)
    {
      hold();
      flag.WAS_HOLD = false;
    }
    previousHoldTime = millis();
  }
  else if (state && flag.PRESSED)
  {
    previousDoubleClickTime =
        previousDebounceTime = millis();
    flag.PRESSED = false;
    flag.WAS_RELEASED = true;
    if (released)
    {
      released();
      flag.WAS_RELEASED = false;
    }
  }
}

bool Button::state()
{
  return flag.PRESSED;
}

bool Button::wasPressed()
{
  if (flag.WAS_PRESSED)
  {
    flag.WAS_PRESSED = false;
    return true;
  }
  else
    return false;
}

bool Button::wasReleased()
{
  if (flag.WAS_RELEASED)
  {
    flag.WAS_RELEASED = false;
    return true;
  }
  else
    return false;
}

bool Button::wasHold()
{
  if (flag.WAS_HOLD)
  {
    flag.WAS_HOLD = false;
    return true;
  }
  else
    return false;
}

bool Button::wasDoubleClick()
{
  if (flag.WAS_DOUBLE_CLICK)
  {
    flag.WAS_DOUBLE_CLICK = false;
    return true;
  }
  else
    return false;
}

void Button::setDebounce(uint16_t timeDebounce)
{
  if (timeDebounce <= MAX_TIME_DEBOUNCE)
    this->timeDebounce = timeDebounce;
  else
    this->timeDebounce = MAX_TIME_DEBOUNCE;
}

void Button::setTimeHold(uint16_t timeHold)
{
  if (timeHold <= MAX_TIME_HOLD)
    this->timeHold = timeHold;
  else
    this->timeHold = MAX_TIME_HOLD;
}

void Button::setTimeDoubleClick(uint16_t timeDoubleClick)
{
  if (timeDoubleClick <= MAX_TIME_DOUBLE_CLICK)
    this->timeDoubleClick = timeDoubleClick;
  else
    this->timeDoubleClick = MAX_TIME_DOUBLE_CLICK;
}

Button &Button::setHandler(handler oneClick)
{
  this->oneClick = oneClick;
  return *this;
}

Button &Button::setHandlerHold(handler hold)
{
  this->hold = hold;
  return *this;
}

Button &Button::setHandlerReleased(handler released)
{
  this->released = released;
  return *this;
}

Button &Button::setHandlerDoubleClick(handler doubleClick)
{
  this->doubleClick = doubleClick;
  return *this;
}
