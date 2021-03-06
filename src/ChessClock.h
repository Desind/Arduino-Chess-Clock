#ifndef CLOCK_H
#define CLOCK_H

#include <Arduino.h>

#define MODE_SUDDEN_DEATH 0
#define MODE_INCREMENT 1
#define MODE_DELAY 2
#define MODE_HOURGLASS 3
#define MODE_BRONSTEIN 4
#define MODE_TOURNAMENT 5

#define PLAYER_LEFT -1
#define PLAYER_RIGHT 1
#define PLAYER_NONE 0

#define PRESET_1i0 0
#define PRESET_1i1 1
#define PRESET_2i0 2
#define PRESET_2i1 3
#define PRESET_3i0 4
#define PRESET_3i2 5
#define PRESET_5i0 6
#define PRESET_5i5 7
#define PRESET_10i0 8
#define PRESET_10i5 9
#define PRESET_15i0 10
#define PRESET_15i10 11
#define PRESET_20i0 12
#define PRESET_30i0 13
#define PRESET_45i45 14
#define PRESET_60i0 15
#define PRESET_90i0 16
#define PRESET_90c40a30i30 17
#define PRESET_120c40a60c60a40i0 18

class ChessClock
{
    private:
        uint32_t leftPlayerTime;                    //TIME LEFT FOR LEFT PLAYER
        uint32_t rightPlayerTime;                   //TIME LEFT FOR RIGHT PLAYER
        uint32_t leftPlayerMoves;                   //MOVE COUNT FOR LEFT PLAYER    
        uint32_t rightPlayerMoves;                  //MOVE COUNT FOR RIGHT PLAYER
        int8_t currentPlayer;                       //TURN OF WHICH PLAYER IS NOW
        byte gameResult;                            //RESULT OF THE GAME (GAME ENDED / GAME RUNNING)
        byte clockMode;                             //CURRENT MODE OF THE CLOCK
        uint32_t incrementValue;                    //VALUE OF INCREMENT TIME
        uint32_t delayValue;                        //VALUE OF DELAY TIME
        uint32_t delayLeft;                         //HOW MUCH DELAY IS LEFT FOR CURRENT PLAYER
        uint32_t bronsteinLeft;                     //START OF LEFT PLAYER TURN - NEEDED TO CALCULATE TIME ADDED AFTER TURN
        uint32_t bronsteinValue;                    //VALUE OF BRONSTEIN INCREMENT TIME
        uint32_t tournamentMoveCap;                 //HOW MANY MOVES UNTIL ADDITIONAL TIME
        uint32_t tournamentPreCapIncrement;         //INCREMENT BEFORE MOVE CAP
        uint32_t tournamentAdditionalTimeAfterCap;  //ADDITIONAL TIME ADDED TO TIMER AFTER MOVE CAP 
        uint32_t tournamentPostCapIncrement;        //INCREMENT AFTER MOVE CAP
        uint32_t tournamentMoveCap2;
        uint32_t tournamentPostCap2Increment;
        uint32_t tournamentAdditionalTimeAfterCap2;
        boolean soundIndicatorEnabled;              //SOUND MOVE INDICATOR TOGGLE
        boolean moveCounterEnabled;                 //VALUE TO DETERMINE IF THERE IS NEED TO DRAW MOVE COUNTER

    public:

        ChessClock();

        void setLeftPlayerTime(uint32_t time);
        void setRightPlayerTime(uint32_t time);
        void setLeftPlayerMoves(uint32_t moves);
        void setRightPlayerMoves(uint32_t moves);
        void setClockMode(byte clockMode);
        void setIncrementValue(uint32_t value);
        void setDelayValue(uint32_t value);
        void setBronsteinValue(uint32_t value);
        void setTournamentMoveCap(uint32_t value);
        void setTournamentPreCapIncrement(uint32_t value);
        void setTournamentAdditionalTimeAfterCap(uint32_t value);
        void setTournamentPostCapIncrement(uint32_t value);
        void setTournamentMoveCap2(uint32_t value);
        void setTournamentPostCap2Increment(uint32_t value);
        void setTournamentAdditionalTimeAfterCap2(uint32_t value);
        void setCurrentPlayer(int8_t player); 
        void switchSoundIndicator(boolean mode);
        void switchMoveCounter(boolean mode);

        void timeUpdate(uint32_t time); //UPDATE CLOCK TIME (SUBTRACT TIME FROM CURRENT PLAYER)
        void playerMove(int8_t player);   //SET CURRENT MOVE TO OPPONENT (ADD OVERDUE TIME FROM INCREMENT)

        void loadPreset(byte preset);   //LOAD CHESS CLOCK PRESET FROM MEMORY

        void saveToEEPROM(uint8_t slot);
        void loadFromEEPROM(uint8_t slot);

        uint32_t getLeftPlayerTime();
        uint32_t getRightPlayerTime();
        uint32_t getLeftPlayerMoves();
        uint32_t getRightPlayerMoves();
        int8_t getCurrentPlayer();
        byte getGameResult();
        byte getClockMode();
        uint32_t getIncrementValue();
        uint32_t getDelayValue();
        uint32_t getDelayLeft();
        uint32_t getBronsteinValue();
        uint32_t getTournamentMoveCap();          
        uint32_t getTournamentPreCapIncrement();       
        uint32_t getTournamentAdditionalTimeAfterCap(); 
        uint32_t getTournamentPostCapIncrement();
        uint32_t getTournamentMoveCap2();
        uint32_t getTournamentPostCap2Increment();
        uint32_t getTournamentAdditionalTimeAfterCap2();
        boolean getSoundIndicatorEnabled();
        boolean getMoveCounterEnabled();

};

#endif