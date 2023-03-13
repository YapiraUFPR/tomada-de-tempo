from enum import Enum
from time import time_ns
from random import randrange

class TimingArc:
    """A state machine that does the timekeeping for the line follower course.\n
        Params:
        * treshhold == the minimum distance the sensors need to register to consider that the robot crossed that arc"""
    class LineState(Enum):
        STANDBY = 0
        READY = 1
        TIMING = 2
        FINISHED = 3

    def __init__(self, treshhold):
        self.treshhold = treshhold

        self.state = self.LineState.STANDBY
        self.start_ts = [0, 0, 0]
        self.time_elaps = [0, 0, 0]
        self.cts = 0

    def __fullReset__(self):
        self.cts = 0
        self.start_ts = [0, 0, 0]
        self.time_elaps = [0, 0, 0]
        self.state = self.LineState.STANDBY

    def __initTrs__(self, begin):
        if begin:
            self.start_ts[self.cts] = 0
            self.time_elaps[self.cts] = 0
            return self.LineState.READY

        return self.LineState.STANDBY

    def __startTrs__(self, slus, ctn):
        if ctn:
            return self.LineState.FINISHED

        for slu in slus:
            if (slu != 0) and (slu < self.treshhold):
                self.start_ts[self.cts] = time_ns()
                return self.LineState.TIMING

        return self.LineState.READY
    
    def __countTrs__(self, flus, ctn):
        if ctn:
            return self.LineState.FINISHED

        for flu in flus:
            if (flu != 0) and (flu < self.treshhold):
                self.start_ts[self.cts] = 0
                return self.LineState.FINISHED

        self.time_elaps[self.cts] = time_ns() - self.start_ts[self.cts]
        return self.LineState.TIMING

    def __finishTrs__(self, begin):
        if begin:
            self.cts += 1
            if self.cts >= 3:
                self.__fullReset__()
            return self.LineState.STANDBY

        return self.LineState.FINISHED
    
    def update(self, slus , flus, cnt=True, reset=False):
        """Make the state transitions"""
        if reset:
            self.__fullReset__()
            return

        match self.state:
            case self.LineState.STANDBY:
                self.state = self.__initTrs__(cnt)
            case self.LineState.READY:
                self.state = self.__startTrs__(slus, cnt)
            case self.LineState.TIMING:
                self.state = self.__countTrs__(flus, cnt)
            case self.LineState.FINISHED:
                self.state = self.__finishTrs__(cnt)

    def getTimeElaps(self):
        """Convert the timestamps to a printable format 'MIN:SEC:MILLIS'"""
        form_ts = []
        for i in range(0, 3):
            millis = self.time_elaps[i] // 1000000
            sec = millis // 1000
            min = sec // 60

            dmillis = "{:03d}".format(millis%1000)
            dsec = "{:02d}".format(sec%60)
            dmin = "{:02d}".format(min)

            form_ts.append(f"{dmin}:{dsec}:{dmillis}")

        return form_ts

    def getState(self):
        return self.state.name

def fakeReading():
    """Generate a fake sensor reading, just for tests"""
    rand = randrange(1, 10000)
    if rand <= 500:
        return "5.1 23.1"
    
    if rand >= 9995:
        return "22.3 5.2"

    return "22.3 23.1"