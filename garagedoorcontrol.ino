int _doorTopSwitch = D0;
int _doorBotSwitch = D1;
int _toggleButton  = D2;

// version

const int VERSION = 2;

// door and switch states

const int UNKNOWN = -1;
const int MOVING  = 2;
const int OPEN    = 3;
const int CLOSED  = 4;
const int FAILURE = 99;
const int OKAY    = 100;

int TopSwitch = UNKNOWN;
int BotSwitch = UNKNOWN;

int _led = D7;
int ledFlip = LOW;

int _actions = 0;

void setup() {
    pinMode(_led, OUTPUT);                  // flip it when something happens just to show things are happening
    pinMode(_toggleButton, OUTPUT);
    pinMode(_doorTopSwitch, INPUT_PULLDOWN);
    pinMode(_doorBotSwitch, INPUT_PULLDOWN);
    
    // Maybe use interrupts for something sometime later...?
    // attachInterrupt(_doorTopSwitch, doorTopSwitch, CHANGE);
    // Spark.publish()...
    
    // Spark.variable("TopSwitch", &TopSwitch, INT);       // uncomment for debugging access
    // Spark.variable("BotSwitch", &BotSwitch, INT);       // uncomment for debugging access
    
    // accessible functions
    
    Spark.function("state", getState);
    Spark.function("action", doAction);
}

void loop() {
    TopSwitch = digitalRead(_doorTopSwitch);
    BotSwitch = digitalRead(_doorBotSwitch);
}

// ----------------------------------------- INTERNAL FUNCTIONS

void _flipYoShizzle() {
    ledFlip = !ledFlip;
    digitalWrite(_led, ledFlip);
    _actions++;
}

int _getDoorState() {
    if (TopSwitch == HIGH && BotSwitch == LOW) {
        return OPEN;
    } else if (TopSwitch == LOW && BotSwitch == HIGH) {
        return CLOSED;
    } else {
        return MOVING;
    }
}

int _sendToggle() {
    digitalWrite(_toggleButton, HIGH);
    delay(1000);
    digitalWrite(_toggleButton, LOW);
}

int _setDoorOpen() {
    int state = _getDoorState();
    
    if (state == CLOSED) {
        _sendToggle();
        return OKAY;
    } else {
        if (state == OPEN) {
            return OPEN;
        } else {
            return MOVING;
        }
    }
}

int _setDoorClose() {
    int state = _getDoorState();
    
    if (state == OPEN) {
        _sendToggle();
        return OKAY;
    } else {
        if (state == CLOSED) {
            return CLOSED;
        } else {
            return MOVING;
        }
    }
}


// ----------------------------------------- EXPORTED FUNCTIONS

int getState(String args) {
    _flipYoShizzle();
    
    args.trim();
    args.toUpperCase();
    
    if (args.equals("DOOR")) {
        return _getDoorState();
    } else if (args.equals("UPTIME")) {
        return millis();
    } else if (args.equals("VERSION")) {
        return VERSION;
    } else if (args.equals("ACTIONS")) {
        return _actions;
    } else {
        return FAILURE;
    }
}

int doAction(String args) {
    _flipYoShizzle();
    
    args.trim();
    args.toUpperCase();
    
    if (args.equals("OPEN")) {
        return _setDoorOpen();
    } else if (args.equals("CLOSE")) {
        return _setDoorClose();
    } else {
        return FAILURE;
    }
}
