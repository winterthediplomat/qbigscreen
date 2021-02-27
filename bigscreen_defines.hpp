#ifndef BIGSCREEN_DEFINES_H
#define BIGSCREEN_DEFINES_H

enum BigScreenTab {
    Twitch = 0,
    Youtube,
    Television,
    Applications /*,
    Timers*/
};

template <typename T>
struct Option
{
private:
    bool is_none;
    T value_;

public:
    static Option<T> Some(T value) {
        Option o;
        o.is_none = false;
        o.value_ = value;
        return o;
    }

    static Option<T> None() {
        Option o;
        o.is_none = true;
        return o;
    }

    bool isNone() {
        return this->is_none;
    }

    T value() {
        assert(!this->isNone() && "unwrapping a None");
        return this->value_;
    }
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
        assert(!this->isError() && "unwrapping an Err value!");
        return value_;
    }

    E error() {
        assert(this->isError() && "this Result is not an Err!");
        return error_;
    }
};

#endif // BIGSCREEN_DEFINES_H
