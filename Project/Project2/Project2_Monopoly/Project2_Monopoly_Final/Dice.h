#ifndef DICE_H
#define DICE_H

class Dice : protected Player{
private:
    int value;
    static int rolls;
public:
    Dice(){
        value=0;
       
    }
    Dice(int input){
        value=input;
    }
    
    Dice(Dice &obj){
        value=obj.value;
    }
    int getValue(){
        return value;
    }

    virtual void setValue(int inputval){
        if(inputval <0){
            throw "ERROR: The value cannot be zero.";
        }
        else{
        value=inputval;
        rolls++;
        }
    }
    
    void operator = (const Dice &val){
        value=val.value;
    }
    
    int operator + (const Dice &val){
        return value+val.value;
    }
    
    int operator - (const Dice &val){
        return value-val.value;
    }
    
    bool operator == (const Dice& val){
        if(value==val.value){
            return true;
        }
        return false;
    }
    
    ~Dice(){
        cout<<"Thanks for playing. This game ended in "<<rolls/2<<" rolls"<<endl;
    }

   void rollCount();
 
};
#endif