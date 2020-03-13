#include <iostream>
#include <napi.h>
#include <thread>
#include <future>
#include "../simdjson/src/simdjson.h"
#include <chrono>

using namespace simdjson;

class simdjsonJS : public Napi::ObjectWrap<simdjsonJS>
{
public:
  simdjsonJS(const Napi::CallbackInfo &info) : Napi::ObjectWrap<simdjsonJS>(info)
  {
    int res = 0;
    /*std::chrono::steady_clock::time_point startGetFile;
    std::chrono::steady_clock::time_point endGetFile;
    std::chrono::steady_clock::time_point startAlloc;
    std::chrono::steady_clock::time_point endAlloc;
    std::chrono::steady_clock::time_point startParse;
    std::chrono::steady_clock::time_point endParse;*/

    //std::chrono::steady_clock::time_point  start = std::chrono::steady_clock::now();

    Napi::Env env = info.Env();
    constructor.Value().Delete("jsObject");
    constructor.Value().Delete("keys");
    constructor.Value().Delete("length");

    if (info.Length() == 0)
    {
      Napi::Error::New(env, "Json path undefined").ThrowAsJavaScriptException();
    }
    else
    {
      // Todo gérer exceptions si le paramètre est incorrect
      
      //startGetFile = std::chrono::steady_clock::now();

      if(info[0].IsString()) {
        // String passed is considered as JSON Document
        Napi::Value param = info[0].As<Napi::Value>();
        std::string jsonRaw = param.ToString();
        bool alloc = _pj.allocate_capacity(jsonRaw.size()); // allocate memory for parsing up to p.size() bytes
        if (alloc) res = simdjson::json_parse(jsonRaw, _pj); // do the parsing, return 0 on success
      } else if (info[0].IsObject()) {
        Napi::Object obj = info[0].As<Napi::Object>();

        if(obj.Has("path") && obj.Get("path").IsString()) {
          std::string path = obj.Get("path").ToString();
          simdjson::padded_string doc = simdjson::get_corpus(path);
          bool alloc = _pj.allocate_capacity(doc.size()); // allocate memory for parsing up to p.size() bytes
          if (alloc) res = simdjson::json_parse(doc, _pj); // do the parsing, return 0 on success
        } else if(obj.Has("doc") && obj.Get("doc").IsString()) {
          std::string jsonRaw = obj.Get("doc").ToString();
          bool alloc = _pj.allocate_capacity(jsonRaw.size()); // allocate memory for parsing up to p.size() bytes
          if (alloc) res = simdjson::json_parse(jsonRaw, _pj); // do the parsing, return 0 on success
        } else {
          // Throw error : Object is missing parameter path or doc
        }
      }
      //endGetFile = std::chrono::steady_clock::now();
      /*if (res != 0)
      {
        Napi::Error::New(env, "Error parsing:" + simdjson::error_message(res)).ThrowAsJavaScriptException();
      }*/
      
      /*else
      {
        Napi::Error::New(env, "Error allocating memory:").ThrowAsJavaScriptException();
      }(*/
    }
    //auto end = std::chrono::steady_clock::now();
    /*std::cout << "Getting parameter time : "  << std::chrono::duration_cast<std::chrono::nanoseconds>(endGetFile - startGetFile).count() << std::endl;
    std::cout << "Allocation time : "  << std::chrono::duration_cast<std::chrono::nanoseconds>(endAlloc - startAlloc).count() << std::endl;
    std::cout << "Parse time : "  << std::chrono::duration_cast<std::chrono::nanoseconds>(endParse - startParse).count() << std::endl;
    std::cout << "Constructor time: "  << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << std::endl;*/
  }

  Napi::Value toJSON(const Napi::CallbackInfo& info) {
    
    SetJSObjects(info.Env());
    return constructor.Value().Get("jsObject");
  }

  Napi::Value ToStringTag(const Napi::CallbackInfo &info)
  {
    Napi::Env env = info.Env();
    simdjson::ParsedJson::Iterator pjh(_pj);
    std::string parsedJSONToString = "";

    if (!pjh.is_ok())
    {
      Napi::Error::New(env, "Could not iterate parsed result. ").ThrowAsJavaScriptException();
    }
    else
    {
      parsedJSONToString = std::string("\n") + ParsedJSONToString(pjh, "");
    }

    return Napi::String::From(info.Env(), parsedJSONToString);
  }

  //The JSON.parse() method parses a JSON string, constructing the JavaScript value or object described by the string. An optional reviver function can be provided to perform a transformation on the resulting object before it is returned.
  std::string ParsedJSONToString(simdjson::ParsedJson::Iterator &pjh, std::string spaces)
  {
    std::string parsedJSONToString = "";
    if (pjh.is_object())
    {
      parsedJSONToString += "{";
      if (pjh.down())
      {
        parsedJSONToString += spaces + pjh.get_string();
        parsedJSONToString += " : ";
        pjh.next();
        parsedJSONToString += ParsedJSONToString(pjh, spaces); // let us recurse
        while (pjh.next())
        {
          parsedJSONToString += ", \n";
          parsedJSONToString += pjh.get_string();
          parsedJSONToString += ":";
          pjh.next();
          parsedJSONToString += ParsedJSONToString(pjh, spaces); // let us recurse
        }
        pjh.up();
      }
      parsedJSONToString += "}";
    }
    else if (pjh.is_array())
    {
      parsedJSONToString += "[";
      if (pjh.down())
      {
        parsedJSONToString += ParsedJSONToString(pjh, spaces); // let us recurse
        while (pjh.next())
        {
          parsedJSONToString += ", ";
          parsedJSONToString += ParsedJSONToString(pjh, spaces); // let us recurse
        }
        pjh.up();
      }
      parsedJSONToString += "]";
    }
    else if (pjh.is_integer())
    {
      parsedJSONToString += std::to_string(pjh.get_integer());
    }
    else if (pjh.is_double())
    {
      parsedJSONToString += std::to_string(pjh.get_double());
    }
    else if (pjh.is_true())
    {
      parsedJSONToString += "true";
    }
    else if (pjh.is_false())
    {
      parsedJSONToString += "false";
    }
    else
    {
      parsedJSONToString += pjh.get_string(); // just print the lone value
    }

    return parsedJSONToString;
  }

  void Keys(Napi::Env env)
  {
    simdjson::ParsedJson::Iterator pjh(_pj);
    std::string parsedJSONToString = "";
    Napi::Array array = Napi::Array::New(env);
    int i = 0;
    if (!constructor.Value().Has("keys"))
    {
      if (pjh.is_object())
      {
        if (pjh.down())
        {
          array.Set(i, Napi::String::New(env, pjh.get_string()));
          i++;
          pjh.next();
          while (pjh.next())
          {
            array.Set(i, Napi::String::New(env, pjh.get_string()));
            i++;
            pjh.next();
          }
        }
      }
      else if (pjh.is_array())
      {
        if (pjh.down())
        {
          array.Set(i, Napi::Number::New(env, i));
          i++;
          while (pjh.next())
          {
            array.Set(i, Napi::Number::New(env, i));
            i++;
          }
        }
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

  //Length
  Napi::Value Length(const Napi::CallbackInfo &info)
  {
    Napi::Env env = info.Env();
    simdjson::ParsedJson::Iterator pjh(_pj);
    std::string parsedJSONToString = "";
    int i = 0;

    if (!constructor.Value().Has("length"))
    { // calculate only if not already calculated to improve performances on multiple calls
      if (pjh.is_array())
      {
        if (pjh.down())
        {
          i++;
          while (pjh.next())
          {
            i++;
          }
        }
        constructor.Value().Set("length", Napi::Number::From(env, i));
      }
      else
      {
        constructor.Value().Set("length", env.Undefined());
      }
    }
    return constructor.Value().Get("length");
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
      Napi::String jsonPointer = info[0].As<Napi::String>();
      
      // Todo : Valider si le paramètre reçu est une chaîne de caractères
      simdjson::ParsedJson::Iterator pjh(_pj);
      pjh.move_to(jsonPointer);

      return makeJSONObject1Depth(env, pjh);
    }
  }
  

  // todo: implémenter les foncitons de tableau
  // Array.prototype.filter() : https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/filter
  // Array.isArray() : bool : vraiment facile
  // Array.prototype.find() : The find() method returns the value of the first element in the provided array that satisfies the provided testing function.
  // Array.prototype.every() : The every() method tests whether all elements in the array pass the test implemented by the provided function. It returns a Boolean value.
  // Array.prototype.some() : The some() method tests whether at least one element in the array passes the test implemented by the provided function. It returns a Boolean value.
  // Array.prototype.map() : The map() method creates a new array with the results of calling a provided function on every element in the calling array.
  // Array.prototype.reduce() : The reduce() method executes a reducer function (that you provide) on each element of the array, resulting in a single output value.
  // Array.sort?

  // Object.values()
  // Object.getOwnPropertyDescriptor() : La méthode Object.getOwnPropertyDescriptor() renvoie un descripteur de la propriété propre d'un objet (c'est-à-dire une propriété directement présente et pas héritée via la chaîne de prototypes).
  // La méthode Object.values() renvoie un tableau contenant les valeurs des propriétés propres énumérables d'un objet dont l'ordre est le même que celui obtenu avec une boucle for...in (la boucle for-in est différente car elle parcourt également les propriétés héritées).

  // Idée : Partir un thread à chaque appel recursif ou appeler une fonction asynchrone?
  // https://stackoverflow.com/questions/32560290/c-thread-with-a-recursive-function
  void SetJSObjects(Napi::Env env) {
    if (!constructor.Value().Has("jsObject"))
    {
    simdjson::ParsedJson::Iterator pjh(_pj);
    std::string parsedJSONToString = "";
    Napi::Array _jsObject;

    int i = 0;

    if (pjh.is_object()) {
        constructor.Value().Set("jsObject", makeJSONObject1Depth(env, pjh));
    }
    else if (pjh.is_array())
    {
        _jsObject = Napi::Array::New(env);

        if (pjh.down())
        {
          _jsObject.Set(i, makeJSONObject1Depth(env, pjh));
          i++;
          while (pjh.next())
          {
            // todo retourner la valeur de l'objet ou (Object si objet complexe!!!)
            _jsObject.Set(i, makeJSONObject1Depth(env, pjh));
            i++;
          }
        
        constructor.Value().Set("jsObject", _jsObject);
        constructor.Value().Set("length", i);       // Profiter de l'opportunité pour définir la propriété length
      }
    }
    else if (pjh.is_string())
    {
      constructor.Value().Set("jsObject", Napi::String::New(env, pjh.get_string()));
    }
    else if (pjh.is_double())
    {
      constructor.Value().Set("jsObject", Napi::Number::New(env, pjh.get_double()));
    }
    else if (pjh.is_integer())
    {
      constructor.Value().Set("jsObject", Napi::Number::New(env, pjh.get_integer()));
    }
    else
    {
      switch (pjh.get_type())
      {
      case 't':
      {
        constructor.Value().Set("jsObject", Napi::Boolean::New(env, true));
        break;
      }
      case 'f':
      {
        constructor.Value().Set("jsObject", Napi::Boolean::New(env, false));
        break;
      }
      case 'n':
      {
        constructor.Value().Set("jsObject", env.Null()); 
        break;
      }
      default:
        break;
      }
    }
    }
  }

  Napi::Value Iterator(const Napi::CallbackInfo &info)
  {
    Napi::Env env = info.Env();
    simdjson::ParsedJson::Iterator pjh(_pj);

    if (pjh.is_array())
    {
      SetJSObjects(info.Env());
      Napi::Function iterator = Napi::Array::New(info.Env()).Get(Napi::Symbol::WellKnown(env, "iterator")).As<Napi::Function>();
      return iterator.Call(constructor.Value().Get("jsObject"), {});
    }
    else
    {
      // Throw error Object is not iterable
      Napi::Error::New(env, "Object is not iterable").ThrowAsJavaScriptException();
      return Napi::Value(env, env.Null()); // Return null value when error occurs.
    }
  }

  Napi::Value makeJSONObject(Napi::Env env, simdjson::ParsedJson::Iterator &pjh)
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
  }

  struct UserDataHolder
  {
    size_t location;
  };

  Napi::Value ObjectGetter(const Napi::CallbackInfo &info)
  {
    const UserDataHolder *holder = reinterpret_cast<UserDataHolder *>(info.Data());

    simdjson::ParsedJson::Iterator pjh(_pj);
    pjh.move_to_specific_location(holder->location);

    return makeJSONObject1Depth(info.Env(), pjh);
  }

  Napi::PropertyDescriptor getProperty(Napi::Env env, Napi::Object &obj, Napi::String key, simdjson::ParsedJson::Iterator &pjh)
  {
    Napi::PropertyDescriptor pd = Napi::PropertyDescriptor::Value("", env.Null());

    if (pjh.is_object())
    {
      UserDataHolder *holder = new UserDataHolder();
      holder->location = pjh.get_tape_location();

      auto callbackGetter = std::bind(&simdjsonJS::ObjectGetter, this, std::placeholders::_1);
      pd = Napi::PropertyDescriptor::Accessor(env, obj, key, callbackGetter, napi_enumerable, reinterpret_cast<void *>(holder));
    }
    else if (pjh.is_array())
    {
      UserDataHolder *holder = new UserDataHolder();
      holder->location = pjh.get_tape_location();

      auto callbackGetter = std::bind(&simdjsonJS::ObjectGetter, this, std::placeholders::_1);
      pd = Napi::PropertyDescriptor::Accessor(env, obj, key, callbackGetter, napi_enumerable, reinterpret_cast<void *>(holder));
    }
    else if (pjh.is_string())
    {
      pd = Napi::PropertyDescriptor::Value(key, Napi::String::New(env, pjh.get_string()), napi_enumerable);
    }
    else if (pjh.is_double())
    {
      pd = Napi::PropertyDescriptor::Value(key, Napi::Number::New(env, pjh.get_double()), napi_enumerable);
    }
    else if (pjh.is_integer())
    {
      pd = Napi::PropertyDescriptor::Value(key, Napi::Number::New(env, pjh.get_integer()), napi_enumerable);
    }
    else
    {
      switch (pjh.get_type())
      {
      case 't':
      {
        pd = Napi::PropertyDescriptor::Value(key, Napi::Boolean::New(env, true), napi_enumerable);
        break;
      }
      case 'f':
      {
        pd = Napi::PropertyDescriptor::Value(key, Napi::Boolean::New(env, false), napi_enumerable);
        break;
      }
      case 'n':
      {
        pd = Napi::PropertyDescriptor::Value(key, env.Null(), napi_enumerable);
        break;
      }
      default:
        break;
      }
    }

    return pd;
  }

  Napi::Value makeJSONObject1Depth(Napi::Env env, simdjson::ParsedJson::Iterator &pjh)
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
        obj.DefineProperty(getProperty(env, obj, key, pjh));
        while (pjh.next())
        { // ,
          key = Napi::String::New(env, pjh.get_string());
          pjh.next();
          obj.DefineProperty(getProperty(env, obj, key, pjh));
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
        Napi::Value value = makeJSONObject1Depth(env, pjh); // let us recurse
        arr.push_back(value);
        while (pjh.next())
        {                                         // ,
          value = makeJSONObject1Depth(env, pjh); // let us recurse
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
  }

  static void Initialize(Napi::Env env, Napi::Object exports)
  {

    Napi::Function func = DefineClass(env, "simdjson", {
      InstanceMethod("keys", &simdjsonJS::Keys, napi_enumerable), 
      InstanceAccessor("length", &simdjsonJS::Length, nullptr, napi_enumerable),

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
  simdjson::ParsedJson _pj;
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