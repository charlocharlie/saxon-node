
#include "XdmAtomicValueJS.hpp"

namespace saxon_node {
    

        void XdmAtomicValueJS::getStringValue(const v8::FunctionCallbackInfo<v8::Value>& args) {
            XdmAtomicValueJS* xdmValue = node::ObjectWrap::Unwrap<XdmAtomicValueJS>(args.This());
            const char* buffer=xdmValue->value->getStringValue();
            args.GetReturnValue().Set(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(),(char*)buffer, v8::NewStringType::kInternalized).ToLocalChecked());
        };

        void XdmAtomicValueJS::getHead(const v8::FunctionCallbackInfo<v8::Value>& args) {
            XdmAtomicValueJS* xdmValue = node::ObjectWrap::Unwrap<XdmAtomicValueJS>(args.This());
            XdmItem* xdmItem=xdmValue->value->getHead();
            v8::Local<v8::Object> instance=XdmItemJS::Instantiate(args.This());
            //XdmItemJS* xv = node::ObjectWrap::Unwrap<XdmItemJS>(instance);
            XdmItemJS* xv = new XdmItemJS();
            xv->value=xdmItem;
            xv->Wrap(instance);
            args.GetReturnValue().Set(instance);
        };

        void XdmAtomicValueJS::itemAt(const v8::FunctionCallbackInfo<v8::Value>& args) {
            v8::Isolate* isolate = v8::Isolate::GetCurrent();
            v8::Local<v8::Context> context = isolate->GetCurrentContext();
            if (args.Length() != 1 || !args[0]->IsNumber()) {

                v8::Isolate::GetCurrent()->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), "expected an index", v8::NewStringType::kInternalized).ToLocalChecked()));
                args.GetReturnValue().SetUndefined();
                return;

            }
            XdmAtomicValueJS* xdmValue = node::ObjectWrap::Unwrap<XdmAtomicValueJS>(args.This());
            XdmItem* xdmItem=xdmValue->value->itemAt(args[0]->IntegerValue(context).ToChecked());
            v8::Local<v8::Object> instance=XdmItemJS::Instantiate(args.This());
            //XdmItemJS* xv = node::ObjectWrap::Unwrap<XdmItemJS>(instance);
            XdmItemJS* xv = new XdmItemJS();
            xv->value=xdmItem;
            xv->Wrap(instance);
            args.GetReturnValue().Set(instance);
        };

        void XdmAtomicValueJS::size(const v8::FunctionCallbackInfo<v8::Value>& args) {
            XdmAtomicValueJS* xdmValue = node::ObjectWrap::Unwrap<XdmAtomicValueJS>(args.This());
            int num=xdmValue->value->size();
            args.GetReturnValue().Set(v8::Uint32::New(v8::Isolate::GetCurrent(), num));
        };

}
