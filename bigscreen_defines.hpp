#ifndef BIGSCREEN_DEFINES_H
#define BIGSCREEN_DEFINES_H

enum BigScreenTab {
    Twitch = 0,
    Youtube,
    Television,
    Applications
};

template <typename T, typename E>
struct Result
{
   private:
    T value_;
    E error_;
    bool is_err;

public:
    static Result<T, E> Ok(T value){
        Result r;
        r.value_ = value;
        r.is_err = false;
        return r;
    }

    static Result<T, E> Err(E error){
       Result r;
       r.error_ = error;
       r.is_err = true;
       return r;
    }

    bool isError() {
        return this->is_err;
    }

    T value() {
        return value_;
    }

    E error() {
        return error_;
    }
};

#endif // BIGSCREEN_DEFINES_H
