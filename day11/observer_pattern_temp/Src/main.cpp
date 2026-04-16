#include <cstdint>
#include <stdio.h>
#include "STM32F405xx.h"

int g=0;
void delay_ms(uint32_t time)
{
    while(time--)
    {
        for(volatile uint32_t i = 0; i < 5000; i++);
    }
}



// Observer interface
class Observer {
public:
    virtual void update(float temperature) = 0;
};

// Subject class (TemperatureSensor)
class TemperatureSensor {
    float temperature;
    Observer  *observers[10];
    int count;

public:
    TemperatureSensor(int c=0):count(c)
    {}
    void attach(Observer* observer) {
    	if(count<=10)
        observers[count++]=observer;
    	else
    		printf("no space for new observer\n\r");
    }

    void detach(Observer* observer) {

       for(int i=0;i<count;i++)
       {
    	  if(observers[i]==observer)
    	  {
    		  for(int j=i;j<count-1;j++)
    		  {
    			  observers[j]=observers[j+1];
    		  }
    		  break;

    	  }

       }

    }

    void setTemperature(float newTemperature) {
    	g=1;
        if (temperature != newTemperature) {
            temperature = newTemperature;
            notify();
        }
    }

    void notify() {
    	for (int i = 0; i < count; i++) {
    	    observers[i]->update(temperature);

    	}
    }
};

// Concrete Observer: DisplaySystem
class DisplaySystem : public Observer {
public:
    void update(float temperature) override {
       printf( "Display updated: Temperature is %lx\n\r",temperature);
       delay_ms(50);
    }
};

// Concrete Observer: AlarmSystem
class AlarmSystem : public Observer {
public:
    void update(float temperature) override {
        if (temperature > 30.0) { // Example threshold for high temperature
            printf("Alarm triggered! Temperature too high: %lx\n\r",temperature) ;
            delay_ms(50);
        }
    }
};

// Main program to demonstrate the Observer pattern
int main() {
    // Create the subject (sensor)
    TemperatureSensor sensor;

    // Create observers
    DisplaySystem display;
    AlarmSystem alarm;

    // Attach observers to the sensor
    sensor.attach(&display);
    sensor.attach(&alarm);

    // Simulate temperature changes
    printf( "Setting temperatureghh to 25°C\n");
    sensor.setTemperature(25.0); // Normal temperature, only display updates

    printf( "\nSetting temperature to 35°C\n");
    sensor.setTemperature(35.0); // High temperature, triggers both display and alarm
    sensor.detach(&alarm);
    return 0;
}


