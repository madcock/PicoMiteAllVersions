// Abstract flash operations on RP2040/RP2350
// James Churchill 2025

int flash_erase(uint32_t address, uint32_t size_bytes);
int flash_program(uint32_t address, const void* buf, uint32_t size_bytes);
int flash_size(void);
