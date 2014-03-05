int _doorTopSwitch = D0;
int _doorBotSwitch = D1;
int _toggleButton  = D2;

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

void setup() {
    pinMode(_led, OUTPUT);                  // flip it when something happens just to show things are happening
    pinMode(_toggleButton, OUTPUT);
    pinMode(_doorTopSwitch, INPUT_PULLDOWN);
    pinMode(_doorBotSwitch, INPUT_PULLDOWN);
    
    // Maybe use interrupts for something sometime later...?
    // attachInterrupt(_doorTopSwitch, doorTopSwitch, CHANGE);
    
    Spark.variable("TopSwitch", &TopSwitch, INT);       // make available just for debugging, should never need to be exported
    Spark.variable("BotSwitch", &BotSwitch, INT);       // make available just for debugging, should never need to be exported
    Spark.function("toggle", sendToggle);               // make available just for debugging, should never need to be exported
    Spark.function("doorstate", getDoorState);
    Spark.function("dooropen", doorOpen);
    Spark.function("doorclose", doorClose);
}

void loop() {
    TopSwitch = digitalRead(_doorTopSwitch);
    BotSwitch = digitalRead(_doorBotSwitch);
}

// ----------------------------------------- INTERNAL FUNCTIONS

void _flipYoShizzle() {
    ledFlip = !ledFlip;
    digitalWrite(_led, ledFlip);
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

// ----------------------------------------- EXPORTED FUNCTIONS

int sendToggle(String args) {
    _flipYoShizzle();
    
    int doorState = _getDoorState();
    
    if (doorState == OPEN || doorState == CLOSED) {
        // Door is in good state, it's okay to activate the remote
        _sendToggle();
        return OKAY;
    } else {
        // it's not okay to activate the remote
        return FAILURE;
    }
}

int getDoorState(String args) {
    _flipYoShizzle();
    return _getDoorState();
}

int doorOpen(String args) {
    _flipYoShizzle();
    
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

int doorClose(String args) {
    _flipYoShizzle();
    
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
