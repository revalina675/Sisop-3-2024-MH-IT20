
#include <stdio.h>
#include <string.h>

char* Gap(float distance) {
    if (distance < 3.5)
        return "Gogogo";
    else if (distance >= 3.5 && distance <= 10)
        return "Push";
    else
        return "Stay out of trouble";
}

char* Fuel(int percent) {
    if (percent > 80)
        return "Push Push Push";
    else if (percent >= 50 && percent <= 80)
        return "You can go";
    else
        return "Conserve Fuel";
}

char* Tire(int wear) {
    if (wear > 80)
        return "Go Push Go Push";
    else if (wear >= 50 && wear <= 80)
        return "Good Tire Wear";
    else if (wear >= 30 && wear < 50)
        return "Conserve Your Tire";
    else
        return "Box Box Box";
}

char* TireChange(char* current_type) {
    if (strcmp(current_type, "Soft") == 0)
        return "Mediums Ready";
    else if (strcmp(current_type, "Medium") == 0)
        return "Box for Softs";
    else
        return "Unknown tire type";
}
