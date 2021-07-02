#define UPDATE_DATA_EVENT "update_data"
#define UPDATE_DATA_FMT "4u1 distance_traveled 2u1 rpm 2u1 fuel 1u1 speed"
typedef struct {
	uint32_t distance_traveled;
	uint16_t rpm;
	uint16_t fuel;
	uint8_t speed;
} update_data_event_t;

