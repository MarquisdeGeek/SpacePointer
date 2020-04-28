
struct TimeSlot {
    char* name;
};

typedef struct SkyPosition {
    int azimuth;
    int altitude;
} SkyPosition;

struct Planet {
    char* name;
    SkyPosition position[MAX_TIME_SLOTS];
};

struct DataSet {
    TimeSlot timeSlots[MAX_TIME_SLOTS];
    Planet planets[MAX_PLANETS];
};
