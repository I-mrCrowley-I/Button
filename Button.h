///////////////////////////////////////////////////////////////////////////
//                   Class BUTTON                                       //
//                   Version 0.18b                                     //
////////////////////////////////////////////////////////////////////////

//--------------------------- TODO ------------------------------------
//
//--------------------------- Обработчики -----------------------------
//  Новый функционал:
//     Добавить работу кнопки по 0 и 1 >>>
//        Не устойчивая работа по +5В
//
//  +/- Добавить обработку двойного нажатия (Проверить!!!)
//
//  Добавить блокировку одновременного нажатия кнопок
//  Добавить блокировку Release после Hold
//
//---------------------------------------------------------------------
//  Проверка временных интервалов
//---------------------------------------------------------------------

#ifndef _BUTTON_H
#define _BUTTON_H

#include <Arduino.h>

#define DEFAULT_TIME_DEBOUNCE 50
#define MAX_TIME_DEBOUNCE 1000
#define DEFAULT_TIME_HOLD 2000
#define MAX_TIME_HOLD 10000
#define DEFAULT_TIME_DOUBLE_CLICK 200
#define MAX_TIME_DOUBLE_CLICK 1100

#if DEFAULT_TIME_DEBOUNCE >= DEFAULT_TIME_HOLD
#warning The anti-bounce interval cannot be greater than the button holding interval!
#elif DEFAULT_TIME_HOLD <= DEFAULT_TIME_DOUBLE_CLICK
#warning Button hold interval cannot be less than double press interval!
#endif

enum Polarity
{
  NEGATIVE,
  POSITIVE
};

struct Flags
{
  bool PRESSED : 1;
  bool POLARITY : 1;
  bool WAS_HOLD : 1;
  bool WAS_PRESSED : 1;
  bool WAS_RELEASED : 1;
  bool WAS_DOUBLE_CLICK : 1;
};

typedef void (*handler)();

class Button
{
private:
  Flags flag;               //Флаги
  handler oneClick;         //Обработчик одиночный клик
  handler doubleClick;      //Обработчик двойной клик
  handler hold;             //Обработчик удержания кнопки
  handler released;         //Обработчик кнопка отпущена
  uint8_t pin;              //Вход кнопки
  uint16_t timeDebounce;    //Интервал анти-дребезга
  uint16_t timeHold;        //Интервал срабатывания удержания
  uint16_t timeDoubleClick; //Интервал двойного клика

  uint32_t previousDebounceTime;
  uint32_t previousHoldTime;
  uint32_t previousDoubleClickTime;

  void init();          //Инициализация флагов
  bool readPin();       //Состояние кнопки (прямое чтение установленного пина)

public:
  Button();
  Button(uint8_t pin);
  Button(uint8_t, uint16_t debounce);
  Button(uint8_t, uint16_t, handler oneClick);

  void setPin(uint8_t pin);                           //Установка пина кнопки
  void setPolarity(bool polarity);                    //Установка полярности кнопки default:NEGATIVE
  void loop();                                        //Цикл опраса кнопки
  bool state();                                       //Состояние кнопки
  bool wasPressed();                                  //Была нажата кнопка (отсроченный опрос)
  bool wasReleased();                                 //Была отпущена кнопка
  bool wasHold();                                     //Было удержание
  bool wasDoubleClick();                              //Был двойной клик
  void setDebounce(uint16_t timeDebounce);            //Установка времени антидребезга
  void setTimeHold(uint16_t timeHold);                //Установка интервала срабатывания удержания
  void setTimeDoubleClick(uint16_t timeDoubleClick);  //Установка интервала двойного клика
  Button &setHandler(handler oneClick);               //Установка обработчика одиночный клик
  Button &setHandlerHold(handler hold);               //Установка обработчика удержания
  Button &setHandlerReleased(handler released);       //Установка обработчика отпущенной кнопки
  Button &setHandlerDoubleClick(handler doubleclick); //Установка обработчика двойного клика

  ~Button();
};

#endif
