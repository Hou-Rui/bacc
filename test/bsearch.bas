LEFT = 1
RIGHT = 2
EPS = 0.00001
DO
    MID = (LEFT + RIGHT) / 2
    IF MID * MID < 2 THEN
        LEFT = MID
    ELSE
        RIGHT = MID
    END IF
LOOP WHILE RIGHT - LEFT > EPS
PRINT LEFT