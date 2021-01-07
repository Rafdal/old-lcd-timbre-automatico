#include <avr/pgmspace.h>

extern char *__brkval;


int freeMemory()
{
	char top;
	return __brkval ? &top - __brkval : &top - __malloc_heap_start;
}

typedef char header_t[21];

const PROGMEM header_t guardar_text = "Guardar?";

class MemoryManager
{
private:
    uint8_t len;
    char buffer[21];
    header_t hbuffer;
public:
    const header_t *container;
    const char* header;

    MemoryManager();
    ~MemoryManager();

    void set(const header_t* _container);
    void set(const char* _header);
    char* take();
    char* take(uint8_t pointer);
    int sizeOf(uint8_t idx);
    int sizeOf();
};

MemoryManager::MemoryManager()
{
}

MemoryManager::~MemoryManager()
{
}

void MemoryManager :: set(const header_t *_container)
{
    container = _container;
}

void MemoryManager :: set(const char* _header)
{
    header = _header;
}

char* MemoryManager :: take()
{
    strcpy_P(buffer, header);
    return (char*)buffer;
}

char* MemoryManager :: take(uint8_t idx)
{
    memcpy_P(&hbuffer, &container[idx], sizeof(header_t));
    return (char*)hbuffer;
}

int MemoryManager :: sizeOf(uint8_t idx)
{
    return strlen( take(idx) );
}

int MemoryManager :: sizeOf()
{
    return strlen( take() );
}