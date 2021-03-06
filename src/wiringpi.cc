#include <node.h>
#include <wiringPi.h>

using v8::Exception;
using v8::Function;
using v8::FunctionCallbackInfo;
using v8::FunctionTemplate;
using v8::Isolate;
using v8::Local;
using v8::Number;
using v8::Object;
using v8::String;
using v8::Value;

void WiringPi_Setup(const FunctionCallbackInfo<Value>& args){
    Isolate* isolate = args.GetIsolate();
    
    if( -1 == wiringPiSetup() ) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Bad argument type")));
        return;
    }
}

void Pin_Mode(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    
    if( args.Length() != 2 ) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments")));
        return;
    }
    
    if( !args[0]->IsNumber() || !args[1]->IsNumber() ) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Bad arguments")));
        return;
    }
    
    if( args[0]->NumberValue() < 0 ) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Index out of bounds")));
        return;
    }

    pinMode( args[0]->NumberValue(), args[1]->NumberValue() );
}


void Digital_Write(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    
    if( args.Length() != 2 ) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments")));
        return;
    }
    
    if( !args[0]->IsNumber() || !args[1]->IsNumber() ) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Bad argument")));
        return;
    }

    if( args[0]->NumberValue() < 0 ) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Index out of bounds")));
        return;
    }

    digitalWrite( args[0]->NumberValue(), args[1]->NumberValue() );
}

void PWM_Write(const FunctionCallbackInfo<Value>& args){
    Isolate* isolate = args.GetIsolate();

    if( args.Length() != 2 ) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments")));
        return;
    }

    if( !args[0]->IsNumber() || !args[1]->IsNumber() ) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Bad argument")));
        return;
    }

    if( args[0]->NumberValue() < 0 ) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Index out of bounds")));
        return;
    }

    pwmWrite( args[0]->NumberValue(), args[1]->NumberValue() );
}

void Digital_Read(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    if( args.Length() != 1 ) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments")));
        return;
    }

    if( !args[0]->IsNumber() ) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Bad argument")));
        return;
    }

    if( args[0]->NumberValue() < 0 ) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Index out of bounds")));
        return;
    }

    args.GetReturnValue().Set(Number::New(isolate, digitalRead(args[0]->NumberValue())));
}

Local<Object> Mode_Constant(){
    Isolate* isolate = Isolate::GetCurrent();
    Local<Object> obj = Object::New(isolate);
    
    obj->Set(String::NewFromUtf8(isolate, "OUTPUT"), Number::New(isolate, OUTPUT));
    obj->Set(String::NewFromUtf8(isolate, "INPUT"), Number::New(isolate, INPUT));
    obj->Set(String::NewFromUtf8(isolate, "PWM_OUTPUT"), Number::New(isolate, PWM_OUTPUT));
    obj->Set(String::NewFromUtf8(isolate, "GPIO_CLOCK"), Number::New(isolate, GPIO_CLOCK));
    obj->Set(String::NewFromUtf8(isolate, "SOFT_PWM_OUTPUT"), Number::New(isolate, SOFT_PWM_OUTPUT));
    obj->Set(String::NewFromUtf8(isolate, "SOFT_TONE_OUTPUT"), Number::New(isolate, SOFT_TONE_OUTPUT));
    obj->Set(String::NewFromUtf8(isolate, "PWM_TONE_OUTPUT"), Number::New(isolate, PWM_TONE_OUTPUT));

    return obj;
}


Local<Object> Value_Constant(){
    Isolate* isolate = Isolate::GetCurrent();
    Local<Object> obj = Object::New(isolate);
    
    obj->Set(String::NewFromUtf8(isolate, "HIGH"), Number::New(isolate, HIGH));
    obj->Set(String::NewFromUtf8(isolate, "LOW"), Number::New(isolate, LOW));
    
    return obj;
}

void Init(Local<Object> exports, Local<Object> module) {
    Isolate* isolate = Isolate::GetCurrent();

    NODE_SET_METHOD(exports, "setup", WiringPi_Setup);
    NODE_SET_METHOD(exports, "pinMode", Pin_Mode);
    NODE_SET_METHOD(exports, "digitalWrite", Digital_Write);
    NODE_SET_METHOD(exports, "digitalRead", Digital_Read);
    NODE_SET_METHOD(exports, "pwmWrite", PWM_Write);

    exports->Set(String::NewFromUtf8(isolate, "mode"), Mode_Constant());
    exports->Set(String::NewFromUtf8(isolate, "value"), Value_Constant());
}

NODE_MODULE(wiringpi, Init)
