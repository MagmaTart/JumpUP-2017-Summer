/****************************************
*
* Mole Project Source Code
*
* By Soomin Lee
*
* 2017.07.28 : Last modified
*
****************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

#define CPU_CLOCK           16000000

/* To control Mole LEDs easily */
#define DDR_MOLE            DDRB
#define PORT_MOLE           PORTB

/* To control Level LEDs easily */
#define DDR_LEVEL_PORT_C    DDRC
#define DDR_LEVEL_PORT_E    DDRE
#define LEVEL_PORT_C        PORTC
#define LEVEL_PORT_E        PORTE
#define BUZZER              PORT6
#define BUZZER_DELAY        700

#define BAUD_RATE           19200


/* To remember each mole's position */
#define MOLE0               PORT7
#define MOLE1               PORT6
#define MOLE2               PORT5
#define MOLE3               PORT4

volatile unsigned int catched = 0;                       // Catched mole number
volatile unsigned int missed = 0;                        // Missed mole number
volatile unsigned int level = 1;                         // Game Level : High is more difficult
volatile unsigned int next_mole_living_times = 1200;     // Living time of mole that will be setting new 
volatile unsigned int next_mole_set_delay = 1200;        // Delay of set new moles : Use to Level Up

volatile unsigned int mole_status[4] = {0, 0, 0, 0};                        // Is that mole live? (Live : 1, Died : 0)
volatile unsigned int mole_living_times[4] = { 1200, 1200, 1200, 1200 };      // Current Living time of mole in that position

volatile unsigned int new_mole_delay = 1200;             // Delay of set new moles

/* Interrupt Initializing */
void init_interrupt()
{
    /* External Interrupt Initializing */
    /* Use INT0, INT1, INT2, INT3 : PD0. PD1, PD2, PD3 */

    EICRA |= (1<<ISC01) | (0<<ISC00) | (1<<ISC11) | (0<<ISC10) | (1<<ISC21) | (0<<ISC20) | (1<<ISC31) | (0<<ISC30);
    EIMSK |= (1<<INT0) | (1<<INT1) | (1<<INT2) | (1<<INT3);
}

//Low pitch Buzzer
void buzzer()
{
    int i=0;
    while(i<50)
    {
        PORTD |= (1<<BUZZER);
        _delay_us(BUZZER_DELAY);
        PORTD &= ~((0<<BUZZER) | 0xFF);
        _delay_us(BUZZER_DELAY);
        i++;
    }
}

//High pitch Buzzer
void buzzer_high()
{
    int i=0;
    while(i<1000)
    {
        PORTD |= (1<<BUZZER);
        _delay_us(BUZZER_DELAY);
        PORTD &= ~((0<<BUZZER) | 0xff);
        _delay_us(BUZZER_DELAY);
        i++;
    }
}

void kill_mole(int num)
{
    int moles[4] = { MOLE0, MOLE1, MOLE2, MOLE3 };      //To iteration

    if(mole_status[num])
    {
        PORT_MOLE &= ~((1 << moles[num]) & 0xff);
        catched++;
        buzzer();
        mole_status[num] = 0;
        mole_living_times[num] = 0;
    }
}

/* INT0 routine : Mole 0 : Button 0 */
ISR(INT0_vect)
{
    kill_mole(0);
}

/* INT1 routine : Mole 1 : Button 1 */
ISR(INT1_vect)
{
    kill_mole(1);
}

/* INT2 routine : Mole 2 : Button 2 */
ISR(INT2_vect)
{
    kill_mole(2);
}

/* INT3 routine : Mole 3 : Button 3 */
ISR(INT3_vect)
{
    kill_mole(3);
}

/* Set new mole and show */
void new_mole(int new)
{
    mole_status[new] = 1;
    mole_living_times[new] = next_mole_living_times;        //Set new living time at that position

    //Light that position
    switch(new)
    {
        case 0:
        PORT_MOLE |= (1<<MOLE0);
        break;

        case 1:
        PORT_MOLE |= (1<<MOLE1);
        break;

        case 2:
        PORT_MOLE |= (1<<MOLE2);
        break;

        case 3:
        PORT_MOLE |= (1<<MOLE3);
        break;

        default:
        break;
    }
}

/* Kill old mole that over its living time */
void shutdown_mole(int mole_number)
{
    switch(mole_number)
    {
        case 0:
        PORT_MOLE &= ~((1<<MOLE0) & 0xff);
        break;

        case 1:
        PORT_MOLE &= ~((1<<MOLE1) & 0xff);
        break;

        case 2:
        PORT_MOLE &= ~((1<<MOLE2) & 0xff);
        break;

        case 3:
        PORT_MOLE &= ~((1<<MOLE3) & 0xff);
        break;
    }

    missed++;
    mole_status[mole_number] = 0;
}

/* Increase difficulty */
void level_up(void)
{
    switch(level)
    {
        case 1:
        LEVEL_PORT_C |= (1<<PORT6);
        break;

        case 2:
        LEVEL_PORT_C &= ~((1<<PORT6) & 0xff);
        LEVEL_PORT_C |= (1<<PORT7);
        break;

        case 3:
        LEVEL_PORT_C &= ~((1<<PORT7) & 0xff);
        LEVEL_PORT_E |= (1<<PORT6);
        break;

        case 4:
        LEVEL_PORT_E &= ~((1<<PORT6) & 0xff);
        LEVEL_PORT_C |= (1<<PORT6) | (1<<PORT7);
        break;

        case 5:
        LEVEL_PORT_C &= ~((1<<PORT7) & 0xff);
        LEVEL_PORT_E |= (1<<PORT6);
        break;

        case 6:
        LEVEL_PORT_C |= (1<<PORT7);
        break;

        default:
        break;
    }

    level++;

    next_mole_set_delay -= 150;
    next_mole_living_times -= 150;
}

void gameover(void)
{
    int i=0;

    PORT_MOLE |= (1 << PORT4) | (1 << PORT5) | (1 << PORT6) | (1 << PORT7);
    buzzer_high();
    PORT_MOLE &= ~(((1 << PORT4) | (1 << PORT5) | (1 << PORT6) | (1 << PORT7)) & 0xff);
}

int main(void)
{
    /* Lock Interrupt */
    cli();

    int i, new;

    DDR_MOLE = 0xff;
    PORT_MOLE = 0x00;

    DDR_LEVEL_PORT_C |= (1 << PORT6) | (1 << PORT7);    // PC6 : Green, PC7 : Yellow
    DDR_LEVEL_PORT_E |= (1 << PORT6);                   // PE7 : Red
    LEVEL_PORT_C = 0x00;                                // Level show port initializing
    LEVEL_PORT_E = 0x00;

    DDRD |= (1 << BUZZER);                            //Buzzer port setting : PD6

    init_interrupt();

    sei();
    /*Unlock Interrupt */

    /* Start Motion is not implemented, replaced to delay function */
    _delay_ms(3000);

    /* Set start mole and show */
    new_mole(rand() % 4);

    while(1)
    {
        /* Global delay 1ms */
        _delay_ms(1);
        
        for(i=0;i<4;i++)
        {
            /* If current position's mole is live */
            if( mole_status[i] )
            {
                mole_living_times[i]--;

                if( !mole_living_times[i] )
                {
                    shutdown_mole(i);       //Living time over : shutdown mole
                }
            }
            else 
            {
                mole_living_times[i] = 0;
            }
        }

        new_mole_delay--;

        /* If delay time is over */
        if( !new_mole_delay )
        {
            new = rand() % 4;
            
            if( !mole_status[new] )
            {
                new_mole(new);
            }

            new_mole_delay = next_mole_set_delay;
        }

        /* Level Up : Level 0 to 6*/
        if( catched > 0 && catched % 15 == 0 && catched <= 15 * 7)
        {
            catched++;
            level_up();
        }

        if( missed >= 15 )
        {
            gameover();
            break;
        }
    }
}
