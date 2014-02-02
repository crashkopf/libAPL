/*
  debug.h
*/

#define INCLUDE_DEBUG \
	V(peek) \
	V(poke) \
	V(dump)

int peek(int argc, char *argv[], environment * env);
int poke(int argc, char *argv[], environment * env);
int dump(int argc, char *argv[], environment * env);

static const char peek_desc [] PROGMEM = "View contents of a memory location"; 
static const char poke_desc [] PROGMEM = "Modify the contents of a memory location"; 
static const char dump_desc [] PROGMEM = "Print the contents of a range of memory"; 
