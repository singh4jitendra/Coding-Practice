#ifndef CALL_REQUEST_H
#define CALL_REQUEST_H

struct CallRequest {
    int floor;
    enum Direction { UP, DOWN };
    Direction direction;
};

struct InsideLiftRequest {
    int floor;
};

#endif // CALL_REQUEST_H