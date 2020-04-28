// A class-ified version of the original debounce example code
// by David A. Mellis et al, 2006
class Debounce {
public:
    Debounce(int pin)
    {
        buttonPin = pin;
    }

    void setup()
    {
        pinMode(buttonPin, INPUT);
        lastDebounceTime = millis();
        buttonState = LOW;
    }

    int update()
    {
        bool hasChanged = false;
        // read the state of the switch into a local variable:
        int reading = digitalRead(buttonPin);

        // check to see if you just pressed the button
        // (i.e. the input went from LOW to HIGH), and you've waited long enough
        // since the last press to ignore any noise:

        // If the switch changed, due to noise or pressing:
        if (reading != lastButtonState) {
            // reset the debouncing timer
            lastDebounceTime = millis();
        }

        if ((millis() - lastDebounceTime) > debounceDelay) {
            // whatever the reading is at, it's been there for longer than the debounce
            // delay, so take it as the actual current state:
            //Serial.println("debounced passed");

            // if the button state has changed:
            if (reading != buttonState) {                
                buttonState = reading;
                hasChanged = true;
            }
        }

        // save the reading. Next time through the loop, it'll be the lastButtonState:
        lastButtonState = reading;

        return hasChanged;
    }

    int getState()
    {
        return buttonState;
    }

private:
    int buttonPin;
    int buttonState; // the current reading from the input pin
    int lastButtonState = LOW; // the previous reading from the input pin

    // the following variables are unsigned longs because the time, measured in
    // milliservoAltitudes, will quickly become a bigger number than can be stored in an int.
    unsigned long lastDebounceTime = 0; // the last time the output pin was toggled
    unsigned long debounceDelay = 50; // the debounce time; increase if the output flickers
};
