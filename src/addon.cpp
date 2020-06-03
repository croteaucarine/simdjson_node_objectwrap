#include <iostream>
#include <napi.h>
#include <thread>
#include <future>
#include "../simdjson/src/simdjson.h"
#include <chrono>
#include <sstream>

#if (SIMDJSON_CPLUSPLUS < 201703L)
#error simdjson addon requires a compiler compliant with the C++17 standard
#endif

using namespace simdjson;

class simdjsonJS : public Napi::ObjectWrap<simdjsonJS>
{
public:
  simdjsonJS(const Napi::CallbackInfo &info) : Napi::ObjectWrap<simdjsonJS>(info)
  {
    constructor.Value().Delete("jsObject");
    constructor.Value().Delete("keys");
    constructor.Value().Delete("ToStringTag");
    
    if(info.Length() == 1)
    {
      
      const Napi::Value objectToLoad = info[0];
      //dom::parser parser;

      if(objectToLoad.IsString()) {
        // String passed is considered as JSON Document
        _parser.parse(padded_string(objectToLoad.ToString()));
      } else if (objectToLoad.IsObject()) {
        Napi::Object obj = objectToLoad.As<Napi::Object>();

        if(obj.Has("path") && obj.Get("path").IsString()) {
          _parser.load(obj.Get("path").ToString());

        } else if(obj.Has("doc") && obj.Get("doc").IsString()) {
          _parser.parse(padded_string(obj.Get("doc").ToString()));
        } else {
          // Throw error : Object is missing parameter path or doc
          //Napi::Error::New(info.Env(), "missing parameter path or doc").ThrowAsJavaScriptException();
        }

        try {
          if(_parser.doc.root().type() == dom::element_type::ARRAY) {
            info.This().As<Napi::Object>().DefineProperty(
            Napi::PropertyDescriptor::Value("length",
                                            Napi::Number::From(info.Env(),  dom::array(_parser.doc.root()).size()), napi_enumerable));
    
          }
        } catch(const std::exception& e) {}
      }
    } else if(info.Length() > 1) 
    {
      //Todo:throw error
    }
  }

  void Load(const Napi::CallbackInfo& info) {

    if(info.Length() == 1)
    {
      Load(info[0]);
    } else if(info.Length() > 1) 
    {
      //Todo:throw error
    }
  }

  // For master purposes, for production use constructor
  int Load(const Napi::Value objectToLoad) {
    // Todo gérer exceptions si le paramètre est incorrect
      if(objectToLoad.IsString()) {
        // String passed is considered as JSON Document
        _textToParse = padded_string(objectToLoad.ToString());
        
      } else if (objectToLoad.IsObject()) {
        Napi::Object obj = objectToLoad.As<Napi::Object>();

        if(obj.Has("path") && obj.Get("path").IsString()) {
          std::string path = obj.Get("path").ToString();
          _textToParse = get_corpus(path);
          
        } else if(obj.Has("doc") && obj.Get("doc").IsString()) {
          _textToParse = padded_string(obj.Get("doc").ToString());
        } else {
          // Throw error : Object is missing parameter path or doc
        }
      }
      return 0; // todo return error if params are incorrect
  }

  simdjson::padded_string get_corpus(const std::string &filename) {
    std::FILE *fp = std::fopen(filename.c_str(), "rb");
    if (fp != nullptr) {
      std::fseek(fp, 0, SEEK_END);
      size_t len = std::ftell(fp);
      padded_string s(len);
      if (s.data() == nullptr) {
        std::fclose(fp);
        return padded_string(0);
      }
      std::rewind(fp);
      size_t readb = std::fread(s.data(), 1, len, fp);
      std::fclose(fp);
      if (readb != len) {
        return padded_string(0);
      }
      return s;
    }
    return padded_string(0);
  }

  // For master purposes, for production use constructor
  void Parse(const Napi::CallbackInfo& info) {

    if(info.Length() == 0)
    {
      Parse();
    } 
      else  
    {
      //Todo:throw error
    }
  }

  // For master purposes, for production use constructor
  int Parse() {
    dom::parser parser;
    _parser.parse(_textToParse);

    return 0; // todo: return error or success
  }

  Napi::Value toJSON(const Napi::CallbackInfo& info) {
    
    SetJSObjects(info.Env());
    return constructor.Value().Get("jsObject");
  }

  Napi::Value ToStringTag(const Napi::CallbackInfo &info)
  {
    Napi::Env env = info.Env();
    std::string parsedJSONToString = "";
    
    if (!constructor.Value().Has("ToStringTag")) {
      parsedJSONToString = "\n" + ParsedJSONToString(_parser.doc.root(), "");
      constructor.Value().Set("ToStringTag", Napi::String::From(env, parsedJSONToString));
    }

    return constructor.Value().Get("ToStringTag");;
  }

  //The JSON.parse() method parses a JSON string, constructing the JavaScript value or object described by the string. An optional reviver function can be provided to perform a transformation on the resulting object before it is returned.
  std::string ParsedJSONToString(dom::element element, std::string spaces, bool breakline = false)
  {
    int i = 0;
    std::string parsedJSONToString = "";
    std::ostringstream strs;
    std::string_view strElement;

    switch (element.type()) {
      case dom::element_type::ARRAY:
        parsedJSONToString += "[";
        for (dom::element child : dom::array(element)) {
          if(i != 0) {
          parsedJSONToString += ",";
        }
          if(child.type() != dom::element_type::OBJECT) parsedJSONToString += " ";
          parsedJSONToString += ParsedJSONToString(child, spaces, true); // let us recurse
          i++;
        }
        parsedJSONToString += " ]"; 
        break;       
    case dom::element_type::OBJECT:
      // Todo: check spaces
      spaces  = "  " + spaces;
      if(breakline) {
        parsedJSONToString += "\n" + spaces;
      }
      
      parsedJSONToString += "{\n";
      for (dom::key_value_pair field : dom::object(element)) {
        if(i != 0) {
          parsedJSONToString += ",\n";
        }
        parsedJSONToString += spaces + "  ";
        parsedJSONToString += field.key;
        parsedJSONToString += ": ";
        parsedJSONToString += ParsedJSONToString(field.value, "  " + spaces, false);
        i++;
      }
      parsedJSONToString += "\n" + spaces + "}";
      break;
    case dom::element_type::INT64:
      parsedJSONToString += std::to_string(int64_t(element));
      break;
    case dom::element_type::UINT64:
      parsedJSONToString += std::to_string(uint64_t(element));
      break;
    case dom::element_type::DOUBLE:
      
      strs << double(element);
      parsedJSONToString += strs.str(); 
      break;
    case dom::element_type::STRING:
      strElement = std::string_view(element);
      if(strElement.find("'") == std::string::npos) {
        parsedJSONToString +=  "'";
        parsedJSONToString +=  std::string_view(element);
        parsedJSONToString +=  "'";
      } else {
        parsedJSONToString +=  "\"";
        parsedJSONToString +=  std::string_view(element);
        parsedJSONToString +=  "\"";
      }
      break;
    case dom::element_type::BOOL:
      parsedJSONToString += bool(element) ? "true" : "false";
      break;
    case dom::element_type::NULL_VALUE:
      parsedJSONToString += "null";
      break;
    }

    return parsedJSONToString;
  }

  void Keys(Napi::Env env)
  {
    std::string parsedJSONToString = "";
    Napi::Array array = Napi::Array::New(env);
    dom::element element = _parser.doc.root();

    int i = 0;
    if (!constructor.Value().Has("keys"))
    {
      switch (element.type()) {
        case dom::element_type::OBJECT:
          for (dom::key_value_pair field : dom::object(element)) {
            array.Set(i, Napi::String::New(env, std::string(field.key)));
            i++;
          }
          break;
        case dom::element_type::ARRAY:
          /*for (dom::element child : dom::array(element)) {
            array.Set(i, Napi::Number::New(env, i));
            i++;
          }*/
          {
            dom::array arrayElements = dom::array(element);
            dom::array::iterator it = arrayElements.begin();
            while(it != arrayElements.end()) {
                array.Set(i, Napi::Number::New(env, i));
                i++;
                it.operator++();
            }
          }
          break;
        default:
          break;
      }
      constructor.Value().Set("keys", array);
    }
  }

  // Keys
  Napi::Value Keys(const Napi::CallbackInfo &info)
  {
    Napi::Env env = info.Env();
    if (!constructor.Value().Has("keys"))
    {
      simdjsonJS::Keys(env);
    }
    return constructor.Value().Get("keys");
  }

  Napi::Value GetValue(const Napi::CallbackInfo &info)
  {
    Napi::Env env = info.Env();

    if (info.Length() == 0)
    {
      Napi::Error::New(env, "Missing JSON Pointer parameter").ThrowAsJavaScriptException();
      return env.Null();
    } 
    else if(!info[0].IsString())
    {
      Napi::Error::New(env, "JSON Pointer parameter must be a string").ThrowAsJavaScriptException();
      return env.Null();
    }
    else
    {
      std::string jsonPointer = info[0].As<Napi::String>();

      try {
        return makeJSONObject1Depth(env, _parser.doc.root().at(jsonPointer));
      } catch (const std::exception& e) {
        Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
        return Napi::Value(env, env.Null()); 
      }
    }
  }

  void SetJSObjects(Napi::Env env) {
    if (!constructor.Value().Has("jsObject"))
    {
      std::string parsedJSONToString = "";
      Napi::Array jsArray;
      dom::element element = _parser.doc.root();
      int i = 0;

      switch(element.type()) {
        case dom::element_type::OBJECT:
          constructor.Value().Set("jsObject", makeJSONObject1Depth(env, element));
          break;
        case dom::element_type::ARRAY:
          jsArray = Napi::Array::New(env); 
          for (dom::element child : dom::array(element)) {
            jsArray.Set(i,  makeJSONObject1Depth(env, child));
            i++;
          }
          constructor.Value().Set("jsObject", jsArray);
          break;
        case dom::element_type::INT64:
          constructor.Value().Set("jsObject", Napi::Number::New(env, int64_t(element)));
          break;
        case dom::element_type::UINT64:
          constructor.Value().Set("jsObject", Napi::Number::New(env, uint64_t(element)));
          break;
        case dom::element_type::DOUBLE:
          constructor.Value().Set("jsObject", Napi::Number::New(env, double(element)));
          break;
        case dom::element_type::STRING:
          constructor.Value().Set("jsObject", Napi::String::New(env, std::string(element)));
          break;
        case dom::element_type::BOOL:
          constructor.Value().Set("jsObject", Napi::Boolean::New(env, bool(element)));
          break;
        case dom::element_type::NULL_VALUE:
          constructor.Value().Set("jsObject", env.Null()); 
          break;
      }
    }
  }

  Napi::Value Iterator(const Napi::CallbackInfo &info)
  {
    Napi::Env env = info.Env();
    dom::element element = _parser.doc.root();

    if (element.type() == dom::element_type::ARRAY)
    {
      SetJSObjects(info.Env());
      Napi::Function iterator = Napi::Array::New(info.Env()).Get(Napi::Symbol::WellKnown(env, "iterator")).As<Napi::Function>();
      
      // Todo : optimize iterator.Call as it takes huge times each n/th times depending on number of objects.
      return iterator.Call(constructor.Value().Get("jsObject"), {});
    }
    else
    {
      // Throw error Object is not iterable
      Napi::Error::New(env, "Object is not iterable").ThrowAsJavaScriptException();
      return Napi::Value(env, env.Null()); // Return null value when error occurs.
    }
  }

  /*Napi::Value makeJSONObject(Napi::Env env, simdjson::ParsedJson::Iterator &pjh)
  {
    Napi::Value v;
    if (pjh.is_object())
    {
      Napi::Object obj = Napi::Object::New(env); // {
      if (pjh.down())
      {
        // must be a string
        Napi::String key = Napi::String::New(env, pjh.get_string());
        // :
        pjh.next();
        Napi::Value value = makeJSONObject(env, pjh); // let us recurse
        obj.Set(key, value);
        while (pjh.next())
        { // ,
          key = Napi::String::New(env, pjh.get_string());
          pjh.next();
          // :
          value = makeJSONObject(env, pjh); // let us recurse
          obj.Set(key, value);
        }
        pjh.up();
      }
      v = obj; // }
    }
    else if (pjh.is_array())
    {
      std::vector<Napi::Value> arr;
      if (pjh.down())
      {
        // [
        Napi::Value value = makeJSONObject(env, pjh); // let us recurse
        arr.push_back(value);
        while (pjh.next())
        {                                   // ,
          value = makeJSONObject(env, pjh); // let us recurse
          arr.push_back(value);
        }
        pjh.up();
      }
      // ]
      Napi::Array array = Napi::Array::New(env, arr.size());
      for (std::size_t i{0}; i < arr.size(); i++)
        array.Set(i, arr[i]);
      v = array;
    }
    else if (pjh.is_string())
    {
      v = Napi::String::New(env, pjh.get_string());
    }
    else if (pjh.is_double())
    {
      v = Napi::Number::New(env, pjh.get_double());
    }
    else if (pjh.is_integer())
    {
      v = Napi::Number::New(env, pjh.get_integer());
    }
    else
    {
      switch (pjh.get_type())
      {
      case 't':
      {
        v = Napi::Boolean::New(env, true);
        break;
      }
      case 'f':
      {
        v = Napi::Boolean::New(env, false);
        break;
      }
      case 'n':
      {
        v = env.Null();
        break;
      }
      default:
        break;
      }
    }

    return v;
  }*/

  struct UserDataHolder
  {
    size_t json_index;
    dom::element element;
  };

  Napi::Value ObjectGetter(const Napi::CallbackInfo &info)
  {
    const UserDataHolder *holder = reinterpret_cast<UserDataHolder *>(info.Data());

    return makeJSONObject1Depth(info.Env(), holder->element);
  }

  Napi::PropertyDescriptor getProperty(Napi::Env env, Napi::Object &obj, Napi::String key, dom::element element)
  {
    Napi::PropertyDescriptor pd = Napi::PropertyDescriptor::Value("", env.Null());
    UserDataHolder *holder = new UserDataHolder();

    switch(element.type()) {
      case dom::element_type::OBJECT :
      case dom::element_type::ARRAY :
        {
        holder->json_index = 0; // Todo : get element.json_index
        holder->element = element; 

        auto callbackGetter = std::bind(&simdjsonJS::ObjectGetter, this, std::placeholders::_1);
        pd = Napi::PropertyDescriptor::Accessor(env, obj, key, callbackGetter, napi_enumerable, reinterpret_cast<void *>(holder));
        }
        break;
      case dom::element_type::INT64 :
        pd = Napi::PropertyDescriptor::Value(key, Napi::Number::New(env, int64_t(element)), napi_enumerable);
        break;
      case dom::element_type::UINT64 :
        pd = Napi::PropertyDescriptor::Value(key, Napi::Number::New(env, uint64_t(element)), napi_enumerable);
        break;
      case dom::element_type::DOUBLE :
        pd = Napi::PropertyDescriptor::Value(key, Napi::Number::New(env, double(element)), napi_enumerable);
        break;
      case dom::element_type::STRING :
        pd = Napi::PropertyDescriptor::Value(key, Napi::String::New(env, std::string(element)), napi_enumerable);
        break;
      case dom::element_type::BOOL :
        pd = Napi::PropertyDescriptor::Value(key, Napi::Boolean::New(env, bool(element)), napi_enumerable);
        break;
      case dom::element_type::NULL_VALUE :
        pd = Napi::PropertyDescriptor::Value(key, env.Null(), napi_enumerable);
        break;
    }

    return pd;
  }

  Napi::Value makeJSONObject1Depth(Napi::Env env, dom::element element)
  {
    Napi::Value v;
    Napi::Object obj = Napi::Object::New(env);
    Napi::Array array = Napi::Array::New(env);
    int i = 0;

    switch (element.type()) {
      case dom::element_type::ARRAY:
        for (dom::element child : dom::array(element)) {
          array.Set(i, makeJSONObject1Depth(env, child));
          i++;
        }
        v = array;
        break;
      case dom::element_type::OBJECT:
        for (dom::key_value_pair field : dom::object(element)) {

          Napi::String key = Napi::String::New(env, std::string(field.key));
          obj.DefineProperty(getProperty(env, obj, key, field.value));
        }
        v = obj; 
        break;
      case dom::element_type::INT64:
        v = Napi::Number::New(env, int64_t(element));
        break;
      case dom::element_type::UINT64:
        v = Napi::Number::New(env, uint64_t(element));
        break;
      case dom::element_type::DOUBLE:
        v = Napi::Number::New(env, double(element));
        break;
      case dom::element_type::STRING:
        v = Napi::String::New(env, std::string(element));
        break;
      case dom::element_type::BOOL:
        v = Napi::Boolean::New(env, bool(element));
        break;
      case dom::element_type::NULL_VALUE:
        v = env.Null();
        break;
    }
    return v;
  }

  static void Initialize(Napi::Env env, Napi::Object exports)
  {
    Napi::Function func = DefineClass(env, "simdjson", {
      InstanceMethod("load", &simdjsonJS::Load, napi_enumerable), 
      InstanceMethod("parse", &simdjsonJS::Parse, napi_enumerable), 
      InstanceMethod("keys", &simdjsonJS::Keys, napi_enumerable), 

      // conventions
      InstanceMethod("toJSON", &simdjsonJS::toJSON, napi_enumerable), 
      InstanceMethod("getValue", &simdjsonJS::GetValue, napi_enumerable),
      InstanceAccessor(Napi::Symbol::WellKnown(env, "toStringTag"), &simdjsonJS::ToStringTag, nullptr, napi_enumerable), 
      InstanceMethod(Napi::Symbol::WellKnown(env, "iterator"), &simdjsonJS::Iterator, napi_enumerable)
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("simdjson", func);
  }

  void Finalize(Napi::Env env)
  {

    /*if(finalizeCb_.IsEmpty()) {
      return;
    }

    finalizeCb_.Call(env.Global(), {Napi::Boolean::New(env, true)});
    finalizeCb_.Unref();*/
  }

private:
  simdjson::padded_string _textToParse;
  dom::parser _parser;
  static Napi::FunctionReference constructor;
};

// Initialize native add-on
Napi::Object Init(Napi::Env env, Napi::Object exports)
{
  simdjsonJS::Initialize(env, exports);
  return exports;
}

Napi::FunctionReference simdjsonJS::constructor;

// Register and initialize native add-on
NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)