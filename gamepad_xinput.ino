#include <XInput.h>
#include <Bounce2.h>

#define X_LEFT 1
#define Y_LEFT 0
#define X_RIGHT 2
#define Y_RIGHT 3

#define JOY_BUTTON_LEFT 15
#define JOY_BUTTON_RIGHT 14

#define UP 0
#define DOWN 16
#define LEFT 1
#define RIGHT 10

#define A 5
#define B 2
#define X 4
#define Y 3

#define BUMPER_LEFT 9
#define BUMPER_RIGHT 7

#define TRIGER_LEFT 8
#define TRIGER_RIGHT 6


class XButton
{
    uint8_t button;
    
    int pin;

    Bounce2::Button button_pin;

    public:

    // the constructor takes the key_code and the no. of the pin the button is attached to

    XButton(uint8_t _button, int _pin) : button(_button), pin(_pin)
    {}

    // we must call this from setup() to setup the button

    void setup()
    {
        button_pin.attach(pin, INPUT_PULLUP);

        button_pin.interval(5);

        button_pin.setPressedState(LOW);
    }

    // call this from loop(), it detects the button press and presses the key

    void update()
    {
        button_pin.update();

        XInput.setButton(button, button_pin.isPressed());
    }
};


class XJoyStick
{
    int x_pin, y_pin, x, y, rng_min, rng_max;

    uint8_t joy_stick;

    public:

    XJoyStick(uint8_t _joy_stick, int _x_pin, int _y_pin) : joy_stick(_joy_stick), x_pin(_x_pin), y_pin(_y_pin)
    {
        if(_joy_stick == JOY_LEFT)
        {
            rng_min = -32768;
            
            rng_max = 32767;
        }
        else
        {
            // right joystick is rotated 180 degrees while placing

            rng_min = 32767;
            
            rng_max = -32768;
        }
    }

    void update()
    {
        x = analogRead(x_pin);

        y = analogRead(y_pin);

        if(492 < x && x < 532)
        {
            x = 512;
        }

        if(492 < y && y < 532)
        {
            y = 512;
        }

        XInput.setJoystick(joy_stick,
            map(x, 0, 1023, rng_min, rng_max),
            map(y, 0, 1023, rng_max, rng_min));
    }
};

	

// XButton A_Button(BUTTON_A, A), B_Button(BUTTON_B, B), X_Button(BUTTON_X, X), Y_Button(BUTTON_Y, Y);

XButton button_list[] = {
                            {BUTTON_L3, JOY_BUTTON_LEFT},
                            {BUTTON_R3, JOY_BUTTON_RIGHT},

                            {DPAD_UP, UP},
                            {DPAD_DOWN, DOWN},
                            {DPAD_LEFT, LEFT},
                            {DPAD_RIGHT, RIGHT},

                            {BUTTON_A, A},
                            {BUTTON_B, B},
                            {BUTTON_X, X},
                            {BUTTON_Y, Y},

                            {BUTTON_LB, BUMPER_LEFT},
                            {BUTTON_RB, BUMPER_RIGHT},
                            
                            {TRIGGER_LEFT, TRIGER_LEFT},
                            {TRIGGER_RIGHT, TRIGER_RIGHT}
};

XJoyStick Left_Joy(JOY_LEFT, X_LEFT, Y_LEFT), Right_Joy(JOY_RIGHT, X_RIGHT, Y_RIGHT);


void setup() {
    // put your setup code here, to run once:

    XInput.setAutoSend(false);

    XInput.begin();

    for(auto& button : button_list)
    {
        button.setup();
    }
}

void loop() {
  // put your main code here, to run repeatedly:

    Left_Joy.update();
    
    Right_Joy.update();

    for(auto& button : button_list)
    {
        button.update();
    }

    XInput.send();

    delay(10);
}
