#ifndef UNIT_HANDLER_HEADER
#   define UNIT_HANDLER_HEADER

typedef struct unitNode {
    unsigned int id;
    unsigned int sampleTime;
    unsigned int lastTime;
    void (*fun_ptr)(unsigned int);

    bool paused = false;
    struct unitNode *next;
} unitNode;

class UNIT_HANDLER {
    public:
        static UNIT_HANDLER* getInstance( );
        unsigned int add (float frequency, void (*fun)(unsigned int), bool paused = false);
        void stop (unsigned int id);
        void start (unsigned int id);
        void _check ( );
    private:
        unitNode *unitList;
        unitNode *listPointer;
        unsigned int counter;
        static UNIT_HANDLER *_instance;
        UNIT_HANDLER ( );
        void _setPaused (unsigned int id, bool paused);
};

UNIT_HANDLER* UNIT_HANDLER::_instance = 0;

UNIT_HANDLER::UNIT_HANDLER ( ) {
    this->unitList = (unitNode*) malloc(sizeof(unitNode));
    this->listPointer = this->unitList;
    this->counter = 0;
}

UNIT_HANDLER* UNIT_HANDLER::getInstance( )
{
    if ( !_instance )
        _instance = new UNIT_HANDLER();
    return _instance;
}

void UNIT_HANDLER::_setPaused (unsigned int id, bool paused) {
    unitNode *currentNode = this->unitList;
    do {
        if (currentNode->id == id) {
            currentNode->paused = paused;
            currentNode->lastTime = micros( );
            break;
        }
        currentNode = currentNode->next;
    } while (currentNode->next != NULL);
}

void UNIT_HANDLER::stop (unsigned int id) {
    this->_setPaused(id, true);
}

void UNIT_HANDLER::start (unsigned int id) {
    this->_setPaused(id, false);
}

unsigned int UNIT_HANDLER::add (float frequency, void (*fun)(unsigned int), bool paused) {
    // increment counter
    this->counter ++;

    this->listPointer->id = this->counter;
    this->listPointer->sampleTime = (unsigned int) (1.0/frequency*1000*1000);
    // cout << "[Addedd process with sampletime: " << this->listPointer->sampleTime << endl;
    this->listPointer->lastTime = 0;
    this->listPointer->paused = paused;
    this->listPointer->fun_ptr = fun;
    this->listPointer->next = (unitNode*) malloc(sizeof(unitNode));
    this->listPointer = this->listPointer->next;
    this->listPointer->next = NULL;
    return this->counter;
}

void UNIT_HANDLER::_check ( ) {
    unsigned int currentTime = micros( );
    unsigned int elapsedTime;
    unitNode *current = this->unitList;
    //cout << "[check] elapsed time: " << elapsedTime << endl;
    do {
        //cout << "[id:" << current->id << "] sample time: " << current->sampleTime << endl;
        if (!current->paused) {
            elapsedTime = currentTime - current->lastTime;
            if (elapsedTime > current->sampleTime) {
                current->fun_ptr(elapsedTime);
                current->lastTime = currentTime;
            }
        }
        current = current->next;
    }
    while (current->next != NULL);
}

#endif // UNIT_HANDLER_HEADER
