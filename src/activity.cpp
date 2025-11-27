template <typename T1 , typename T2> class dictionary
{
private:
    T1 key;
    T2 value;
public:
    dictionary(T1 k , T2 v)
    {
        key = k;
        value = v;
    }
    T1 getKey() const { return key; }
    T2 getValue() const { return value; } 
    void setValue(T2 v) { value = v; }   
    void setKey(T1 k) { key = k; } 
    bool checkKey(T1 k)
    {
        return key == k;
    }
    dictionary operator+(dictionary d)
    {
        if(checkKey(d.getKey()))
        {
            setValue(getValue() + d.getValue());
            
        }
        else
        {
            dictionary d1(key , value);
            dictionary d2(d.getKey() , d.getValue());
            return d1 + d2;
        }
    }
};
