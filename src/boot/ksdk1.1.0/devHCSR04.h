int takeReading(void);
void lptmr_isr_callback(void);
int hal_tick_get_tick_period_in_ms(void);
void hal_tick_set_handler(void (*handler)(void));