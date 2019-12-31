#pragma once
#include <map>

#include <v8.h>
#include <uv.h>
#include <node.h>
#include <node_object_wrap.h>
#include <string>
#include <cstring>
#include <memory>
#include <jni.h>

#include "XdmItem.h"
#include "XdmValue.h"
#include "XPathProcessor.h"

#include "XdmValueJS.hpp"

namespace saxon_node {

    using namespace v8;

    class SaxonProcessorJS;

    class XPathProcessorJS : public node::ObjectWrap {
    protected:

    public:

        static void Initialize(v8::Local<v8::Object> target) {
            v8::Isolate* isolate = v8::Isolate::GetCurrent();
            v8::Local<v8::Context> context = isolate->GetCurrentContext();
            // instantiate constructor function template
            Local<FunctionTemplate> t = FunctionTemplate::New(v8::Isolate::GetCurrent(), New);
            t->SetClassName(String::NewFromUtf8(v8::Isolate::GetCurrent(), "XPathProcessor", v8::NewStringType::kInternalized).ToLocalChecked());
            t->InstanceTemplate()->SetInternalFieldCount(1);
            Constructor.Reset(v8::Isolate::GetCurrent(), t);
            // member method prototypes
            NODE_SET_PROTOTYPE_METHOD(t, "setBaseURI", setBaseURI);
            NODE_SET_PROTOTYPE_METHOD(t, "evaluate", evaluate);
            NODE_SET_PROTOTYPE_METHOD(t, "evaluateSingle", evaluateSingle);
            NODE_SET_PROTOTYPE_METHOD(t, "setContextItem", setContextItem);
            NODE_SET_PROTOTYPE_METHOD(t, "setContextFile", setContextFile);
            NODE_SET_PROTOTYPE_METHOD(t, "effectiveBooleanValue", effectiveBooleanValue);
            NODE_SET_PROTOTYPE_METHOD(t, "setParameter", setParameter);
            NODE_SET_PROTOTYPE_METHOD(t, "getParameter", getParameter);
            NODE_SET_PROTOTYPE_METHOD(t, "removeParameter", removeParameter);
            NODE_SET_PROTOTYPE_METHOD(t, "setProperty", setProperty);
            NODE_SET_PROTOTYPE_METHOD(t, "declareNamespace", declareNamespace);
            NODE_SET_PROTOTYPE_METHOD(t, "getProperty", getProperty);
            NODE_SET_PROTOTYPE_METHOD(t, "clearParameters", clearParameters);
            // append this function to the target object
            target->Set(String::NewFromUtf8(v8::Isolate::GetCurrent(), "XPathProcessor", v8::NewStringType::kInternalized).ToLocalChecked(), t->GetFunction(context).ToLocalChecked());
        };

        static Local<Object> Instantiate(Local<Object> proc) {
            v8::Isolate* isolate = v8::Isolate::GetCurrent();
            v8::Local<v8::Context> context = isolate->GetCurrentContext();
            const unsigned        argc       = 1;
            Local<Value> argv[1] = {

                proc

            };

            // return new group instance
            return Local<FunctionTemplate>::New(isolate, Constructor)->GetFunction(context).ToLocalChecked()->NewInstance(isolate->GetCurrentContext(), argc, argv).ToLocalChecked();

        };
    private:

        XPathProcessorJS() : XPathProcessorJS(false) {

        };

        XPathProcessorJS(bool l) {

        };

        ~XPathProcessorJS() {
        };
        static Persistent<FunctionTemplate> Constructor;

        static void New(const v8::FunctionCallbackInfo<Value>& args) {
            v8::Isolate* isolate = v8::Isolate::GetCurrent();
            v8::Local<v8::Context> context = isolate->GetCurrentContext();
            // create hdf file object
            XPathProcessorJS* xp;
            if (args.Length() < 1)
                xp = new XPathProcessorJS();
            else
                xp = new XPathProcessorJS(args[1]->ToBoolean(isolate)->Value());

            xp->procJS = args[0]->ToObject(context).ToLocalChecked();
            // unwrap processor object
            xp->proc = ObjectWrap::Unwrap<SaxonProcessorJS>(args[0]->ToObject(context).ToLocalChecked());

            xp->xpathProcessor.reset(xp->proc->processor->newXPathProcessor());
            // extend target object with processor
            xp->Wrap(args.This());

            // attach various properties
            args.This()->Set(String::NewFromUtf8(v8::Isolate::GetCurrent(), "parameters", v8::NewStringType::kInternalized).ToLocalChecked(), Object::New(v8::Isolate::GetCurrent()));
            args.This()->Set(String::NewFromUtf8(v8::Isolate::GetCurrent(), "properties", v8::NewStringType::kInternalized).ToLocalChecked(), Object::New(v8::Isolate::GetCurrent()));
        };

        static void setSourceValue(const v8::FunctionCallbackInfo<Value>& args) {
            v8::Isolate::GetCurrent()->ThrowException(v8::Exception::Error(String::NewFromUtf8(v8::Isolate::GetCurrent(), "unsupported method", v8::NewStringType::kInternalized).ToLocalChecked()));
            args.GetReturnValue().SetUndefined();
        };

        static void setContextItem(const v8::FunctionCallbackInfo<Value>& args) {
            v8::Isolate* isolate = v8::Isolate::GetCurrent();
            v8::Local<v8::Context> context = isolate->GetCurrentContext();
            if (args.Length() != 1 || !args[0]->IsString()) {

                v8::Isolate::GetCurrent()->ThrowException(v8::Exception::Error(String::NewFromUtf8(v8::Isolate::GetCurrent(), "expected xml source as string", v8::NewStringType::kInternalized).ToLocalChecked()));
                args.GetReturnValue().SetUndefined();
                return;

            }
            // the source
            String::Utf8Value theContext(isolate, args[0]->ToString(context).ToLocalChecked());
            //std::cout<<(*source)<<std::endl;
            // unwrap xpathProcessor object
            XPathProcessorJS* xp = ObjectWrap::Unwrap<XPathProcessorJS>(args.This());
            xp->xpathProcessor->setContextItem((XdmItem*)xp->proc->processor->makeStringValue(*theContext));
            args.GetReturnValue().SetUndefined();

        };

        static void setContextFile(const v8::FunctionCallbackInfo<Value>& args) {
            v8::Isolate* isolate = v8::Isolate::GetCurrent();
            v8::Local<v8::Context> context = isolate->GetCurrentContext();
            if (args.Length() != 1 || !args[0]->IsString()) {

                v8::Isolate::GetCurrent()->ThrowException(v8::Exception::Error(String::NewFromUtf8(v8::Isolate::GetCurrent(), "expected context source as path string", v8::NewStringType::kInternalized).ToLocalChecked()));
                args.GetReturnValue().SetUndefined();
                return;

            }
            // the source
            String::Utf8Value contextFile(isolate, args[0]->ToString(context).ToLocalChecked());
            //std::cout<<(*contextFile)<<std::endl;
            // unwrap xpathProcessor object
            XPathProcessorJS* xp = ObjectWrap::Unwrap<XPathProcessorJS>(args.This());
            xp->xpathProcessor->setContextFile(*contextFile);
            args.GetReturnValue().SetUndefined();

        };

        static void setParameter(const v8::FunctionCallbackInfo<Value>& args) {
            v8::Isolate::GetCurrent()->ThrowException(v8::Exception::Error(String::NewFromUtf8(v8::Isolate::GetCurrent(), "unsupported method", v8::NewStringType::kInternalized).ToLocalChecked()));
            args.GetReturnValue().SetUndefined();
        };

        static void getParameter(const v8::FunctionCallbackInfo<Value>& args) {
            v8::Isolate::GetCurrent()->ThrowException(v8::Exception::Error(String::NewFromUtf8(v8::Isolate::GetCurrent(), "unsupported method", v8::NewStringType::kInternalized).ToLocalChecked()));
            args.GetReturnValue().SetUndefined();
        };

        static void removeParameter(const v8::FunctionCallbackInfo<Value>& args) {
            v8::Isolate::GetCurrent()->ThrowException(v8::Exception::Error(String::NewFromUtf8(v8::Isolate::GetCurrent(), "unsupported method", v8::NewStringType::kInternalized).ToLocalChecked()));
            args.GetReturnValue().SetUndefined();
        };

        static void setProperty(const v8::FunctionCallbackInfo<Value>& args) {
            v8::Isolate::GetCurrent()->ThrowException(v8::Exception::Error(String::NewFromUtf8(v8::Isolate::GetCurrent(), "unsupported method", v8::NewStringType::kInternalized).ToLocalChecked()));
            args.GetReturnValue().SetUndefined();
        };

        static void getProperty(const v8::FunctionCallbackInfo<Value>& args) {
            v8::Isolate::GetCurrent()->ThrowException(v8::Exception::Error(String::NewFromUtf8(v8::Isolate::GetCurrent(), "unsupported method", v8::NewStringType::kInternalized).ToLocalChecked()));
            args.GetReturnValue().SetUndefined();
        };

        static void clearParameters(const v8::FunctionCallbackInfo<Value>& args) {
            v8::Isolate::GetCurrent()->ThrowException(v8::Exception::Error(String::NewFromUtf8(v8::Isolate::GetCurrent(), "unsupported method", v8::NewStringType::kInternalized).ToLocalChecked()));
            args.GetReturnValue().SetUndefined();
        };

        static void clearProperties(const v8::FunctionCallbackInfo<Value>& args) {
            v8::Isolate::GetCurrent()->ThrowException(v8::Exception::Error(String::NewFromUtf8(v8::Isolate::GetCurrent(), "unsupported method", v8::NewStringType::kInternalized).ToLocalChecked()));
            args.GetReturnValue().SetUndefined();
        };

        static void setBaseURI(const v8::FunctionCallbackInfo<Value>& args) {
            v8::Isolate* isolate = v8::Isolate::GetCurrent();
            v8::Local<v8::Context> context = isolate->GetCurrentContext();
            if(args.Length()!=1 || !args[0]->IsString() )
            {
                v8::Isolate::GetCurrent()->ThrowException(v8::Exception::Error(String::NewFromUtf8(v8::Isolate::GetCurrent(), "expected base uri as string", v8::NewStringType::kInternalized).ToLocalChecked()));
                args.GetReturnValue().SetUndefined();
            }
            // unwrap xpathProcessor object
            XPathProcessorJS* xp = ObjectWrap::Unwrap<XPathProcessorJS>(args.This());
            // the base uri
            String::Utf8Value baseUri(isolate, args[0]->ToString(context).ToLocalChecked());
            xp->xpathProcessor->setBaseURI((*baseUri));
            if(xp->xpathProcessor->exceptionOccurred() || xp->xpathProcessor->exceptionCount()>0){
                if(xp->xpathProcessor->exceptionCount()==0)xp->xpathProcessor->checkException();
                std::ostringstream ss;
                ss<<"# of exceptions: "<<std::to_string(xp->xpathProcessor->exceptionCount())<<std::endl;
                for(unsigned int exceptionIndex=0;exceptionIndex<xp->xpathProcessor->exceptionCount();exceptionIndex++){
                    ss<<xp->xpathProcessor->getErrorMessage(exceptionIndex)<<std::endl;
                }
                v8::Isolate::GetCurrent()->ThrowException(v8::Exception::Error(String::NewFromUtf8(v8::Isolate::GetCurrent(), ss.str().c_str(), v8::NewStringType::kInternalized).ToLocalChecked()));
                args.GetReturnValue().SetUndefined();
                return;
                
            }
            args.GetReturnValue().SetUndefined();
        };

        static void evaluate(const v8::FunctionCallbackInfo<Value>& args) {
            v8::Isolate* isolate = v8::Isolate::GetCurrent();
            v8::Local<v8::Context> context = isolate->GetCurrentContext();
            if(args.Length()!=1 || !args[0]->IsString())
            {
                v8::Isolate::GetCurrent()->ThrowException(v8::Exception::Error(String::NewFromUtf8(v8::Isolate::GetCurrent(), "expected xpath string", v8::NewStringType::kInternalized).ToLocalChecked()));
                args.GetReturnValue().SetUndefined();
            }
            // unwrap xpathProcessor object
            XPathProcessorJS* xp = ObjectWrap::Unwrap<XPathProcessorJS>(args.This());
            /*Local<Object> parameters=args.This()->Get(String::NewFromUtf8(v8::Isolate::GetCurrent(), "parameters", v8::NewStringType::kInternalized).ToLocalChecked())->ToObject(context).ToLocalChecked();
            Local<Array> parameterNames=parameters->GetOwnPropertyNames();
            for(uint32_t index=0;index<parameterNames->Length();index++)
            {
//                            std::cout<<" "<<parameterNames->IsNull()<<" "<<parameterNames->IsString()<<" "<<parameterNames->IsArray()<<" "<<parameterNames->Length()<<std::endl;
                Local<Object> obj=parameterNames->Get(index)->ToObject(context).ToLocalChecked();
//                            std::cout<<"obj "<<obj->IsString()<<std::endl;
                String::Utf8Value pn(isolate, obj->ToString(context).ToLocalChecked());
                String::Utf8Value pnValue(isolate, parameters->Get(parameterNames->Get(index)->ToString(context).ToLocalChecked())->ToString(context).ToLocalChecked());
                //std::cout<<(*pn)<<" "<<(*pnValue)<<std::endl;
                //@todo xp->xpathProcessor->setParameter(*pn, new XdmValue(*pnValue));
            }*/
            // the source
            String::Utf8Value xpath(isolate, args[0]->ToString(context).ToLocalChecked());
            //std::cout<<(*xpath)<<std::endl;
            XdmValue * results=xp->xpathProcessor->evaluate((*xpath));
            if(xp->xpathProcessor->exceptionOccurred() || xp->xpathProcessor->exceptionCount()>0){
                if(xp->xpathProcessor->exceptionCount()==0)xp->xpathProcessor->checkException();
                std::ostringstream ss;
                ss<<"# of exceptions: "<<std::to_string(xp->xpathProcessor->exceptionCount())<<std::endl;
                for(unsigned int exceptionIndex=0;exceptionIndex<xp->xpathProcessor->exceptionCount();exceptionIndex++){
                    ss<<xp->xpathProcessor->getErrorMessage(exceptionIndex)<<std::endl;
                }
                v8::Isolate::GetCurrent()->ThrowException(v8::Exception::Error(String::NewFromUtf8(v8::Isolate::GetCurrent(), ss.str().c_str(), v8::NewStringType::kInternalized).ToLocalChecked()));
                args.GetReturnValue().SetUndefined();
                return;
                
            }
            
            Local<Object> instance=XdmValueJS::Instantiate(args.This());
            XdmValueJS* xdmValue = new XdmValueJS();
            xdmValue->value=results;
            xdmValue->Wrap(instance);
            args.GetReturnValue().Set(instance);
        };

        static void evaluateSingle(const v8::FunctionCallbackInfo<Value>& args) {
            v8::Isolate* isolate = v8::Isolate::GetCurrent();
            v8::Local<v8::Context> context = isolate->GetCurrentContext();
            Local<Object> parameters=args.This()->Get(String::NewFromUtf8(v8::Isolate::GetCurrent(), "parameters", v8::NewStringType::kInternalized).ToLocalChecked())->ToObject(context).ToLocalChecked();
            Local<Array> parameterNames=parameters->GetOwnPropertyNames(context).ToLocalChecked();
            Local<Object> properties=args.This()->Get(String::NewFromUtf8(v8::Isolate::GetCurrent(), "properties", v8::NewStringType::kInternalized).ToLocalChecked())->ToObject(context).ToLocalChecked();
            Local<Array> propertyNames=properties->GetOwnPropertyNames(context).ToLocalChecked();
            switch(args.Length())
            {
                case 1:
                    if(args[0]->IsString())
                    {
                        // unwrap xpathProcessor object
                        XPathProcessorJS* xp = ObjectWrap::Unwrap<XPathProcessorJS>(args.This());
                        //if(args.This()->IsDirty())
                        {
                            for(uint32_t index=0;index<parameterNames->Length();index++)
                            {
    //                            std::cout<<" "<<parameterNames->IsNull()<<" "<<parameterNames->IsString()<<" "<<parameterNames->IsArray()<<" "<<parameterNames->Length()<<std::endl;
                                Local<Object> obj=parameterNames->Get(index)->ToObject(context).ToLocalChecked();
    //                            std::cout<<"obj "<<obj->IsString()<<std::endl;
                                String::Utf8Value pn(isolate, obj->ToString(context).ToLocalChecked());
                                String::Utf8Value pnValue(isolate, parameters->Get(parameterNames->Get(index)->ToString(context).ToLocalChecked())->ToString(context).ToLocalChecked());
                                //std::cout<<(*pn)<<" "<<(*pnValue)<<std::endl;
                                //@todo xp->xpathProcessor->setParameter(*pn, new XdmValue(*pnValue));
                            }
                        }
                        // the source
                        String::Utf8Value source(isolate, args[0]->ToString(context).ToLocalChecked());
                        XdmItem* buffer=xp->xpathProcessor->evaluateSingle((*source));
                        if(xp->xpathProcessor->exceptionOccurred() || xp->xpathProcessor->exceptionCount()>0){
                            if(xp->xpathProcessor->exceptionCount()==0)xp->xpathProcessor->checkException();
                            std::ostringstream ss;
                            ss<<"# of exceptions: "<<std::to_string(xp->xpathProcessor->exceptionCount())<<std::endl;
                            for(unsigned int exceptionIndex=0;exceptionIndex<xp->xpathProcessor->exceptionCount();exceptionIndex++){
                                ss<<xp->xpathProcessor->getErrorMessage(exceptionIndex)<<std::endl;
                            }
                            v8::Isolate::GetCurrent()->ThrowException(v8::Exception::Error(String::NewFromUtf8(v8::Isolate::GetCurrent(), ss.str().c_str(), v8::NewStringType::kInternalized).ToLocalChecked()));
                            args.GetReturnValue().SetUndefined();
                            break;
                            
                        }
                        args.GetReturnValue().Set(node::Buffer::New(v8::Isolate::GetCurrent(), (char*)buffer->getStringValue(), std::strlen((char*)buffer->getStringValue())).ToLocalChecked());
                    }
                    break;
            }
//            v8::Isolate::GetCurrent()->ThrowException(v8::Exception::Error(String::NewFromUtf8(v8::Isolate::GetCurrent(), "arguments aren't strings", v8::NewStringType::kInternalized).ToLocalChecked()));
//            args.GetReturnValue().SetUndefined();
//            return;
        };

        static void effectiveBooleanValue(const v8::FunctionCallbackInfo<Value>& args) {
            v8::Isolate* isolate = v8::Isolate::GetCurrent();
            v8::Local<v8::Context> context = isolate->GetCurrentContext();
            if(args.Length()!=1 || !args[0]->IsString() )
            {
                v8::Isolate::GetCurrent()->ThrowException(v8::Exception::Error(String::NewFromUtf8(v8::Isolate::GetCurrent(), "expected base uri as string", v8::NewStringType::kInternalized).ToLocalChecked()));
                args.GetReturnValue().SetUndefined();
            }
            // unwrap xpathProcessor object
            XPathProcessorJS* xp = ObjectWrap::Unwrap<XPathProcessorJS>(args.This());
            // the base uri
            String::Utf8Value xPathStr(isolate, args[0]->ToString(context).ToLocalChecked());
            bool effective=xp->xpathProcessor->effectiveBooleanValue((*xPathStr));
            if(xp->xpathProcessor->exceptionOccurred() || xp->xpathProcessor->exceptionCount()>0){
                if(xp->xpathProcessor->exceptionCount()==0)xp->xpathProcessor->checkException();
                std::ostringstream ss;
                ss<<"# of exceptions: "<<std::to_string(xp->xpathProcessor->exceptionCount())<<std::endl;
                for(unsigned int exceptionIndex=0;exceptionIndex<xp->xpathProcessor->exceptionCount();exceptionIndex++){
                    ss<<xp->xpathProcessor->getErrorMessage(exceptionIndex)<<std::endl;
                }
                v8::Isolate::GetCurrent()->ThrowException(v8::Exception::Error(String::NewFromUtf8(v8::Isolate::GetCurrent(), ss.str().c_str(), v8::NewStringType::kInternalized).ToLocalChecked()));
                args.GetReturnValue().SetUndefined();
                return;
                
            }
            args.GetReturnValue().Set(effective);
        };

        static void declareNamespace(const v8::FunctionCallbackInfo<Value>& args) {
            v8::Isolate* isolate = v8::Isolate::GetCurrent();
            v8::Local<v8::Context> context = isolate->GetCurrentContext();
            if(args.Length()!=2 || !args[0]->IsString()|| !args[1]->IsString())
            {
                v8::Isolate::GetCurrent()->ThrowException(v8::Exception::Error(String::NewFromUtf8(v8::Isolate::GetCurrent(), "expected prefix and uri strings", v8::NewStringType::kInternalized).ToLocalChecked()));
                args.GetReturnValue().SetUndefined();
            }
            // unwrap xpathProcessor object
            XPathProcessorJS* xp = ObjectWrap::Unwrap<XPathProcessorJS>(args.This());
            /*Local<Object> parameters=args.This()->Get(String::NewFromUtf8(v8::Isolate::GetCurrent(), "parameters", v8::NewStringType::kInternalized).ToLocalChecked())->ToObject(context).ToLocalChecked();
            Local<Array> parameterNames=parameters->GetOwnPropertyNames();
            for(uint32_t index=0;index<parameterNames->Length();index++)
            {
//                            std::cout<<" "<<parameterNames->IsNull()<<" "<<parameterNames->IsString()<<" "<<parameterNames->IsArray()<<" "<<parameterNames->Length()<<std::endl;
                Local<Object> obj=parameterNames->Get(index)->ToObject(context).ToLocalChecked();
//                            std::cout<<"obj "<<obj->IsString()<<std::endl;
                String::Utf8Value pn(isolate, obj->ToString(context).ToLocalChecked());
                String::Utf8Value pnValue(isolate, parameters->Get(parameterNames->Get(index)->ToString(context).ToLocalChecked())->ToString(context).ToLocalChecked());
                //std::cout<<(*pn)<<" "<<(*pnValue)<<std::endl;
                //@todo xp->xpathProcessor->setParameter(*pn, new XdmValue(*pnValue));
            }*/
            // the source
            String::Utf8Value prefix(isolate, args[0]->ToString(context).ToLocalChecked());
            String::Utf8Value uri(isolate, args[0]->ToString(context).ToLocalChecked());
            xp->xpathProcessor->declareNamespace((*prefix), (*uri));
            if(xp->xpathProcessor->exceptionOccurred() || xp->xpathProcessor->exceptionCount()>0){
                if(xp->xpathProcessor->exceptionCount()==0)xp->xpathProcessor->checkException();
                std::ostringstream ss;
                ss<<"# of exceptions: "<<std::to_string(xp->xpathProcessor->exceptionCount())<<std::endl;
                for(unsigned int exceptionIndex=0;exceptionIndex<xp->xpathProcessor->exceptionCount();exceptionIndex++){
                    ss<<xp->xpathProcessor->getErrorMessage(exceptionIndex)<<std::endl;
                }
                v8::Isolate::GetCurrent()->ThrowException(v8::Exception::Error(String::NewFromUtf8(v8::Isolate::GetCurrent(), ss.str().c_str(), v8::NewStringType::kInternalized).ToLocalChecked()));
                args.GetReturnValue().SetUndefined();
                return;
                
            }
            
            args.GetReturnValue().SetUndefined();
        };

        static void exceptionOccurred(const v8::FunctionCallbackInfo<Value>& args) {
            v8::Isolate::GetCurrent()->ThrowException(v8::Exception::Error(String::NewFromUtf8(v8::Isolate::GetCurrent(), "unsupported method", v8::NewStringType::kInternalized).ToLocalChecked()));
            args.GetReturnValue().SetUndefined();
        };

    private:
        Local<Object> procJS;
        SaxonProcessorJS* proc;
        std::shared_ptr<XPathProcessor> xpathProcessor;
        XdmValue* value;

    };

};
