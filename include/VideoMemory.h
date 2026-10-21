#ifndef VIDMEM
#define VIDMEM

#define MAX_COLORS 256
#define NO_COLOR "\x1b[0m"

void DrawMemory(Processor_t *Processor);

#ifdef GRAPHICS_FLAG
#define DRAW(Processor) DrawMemory(&(Processor))
#else
#define DRAW(Processor)
#endif

#endif