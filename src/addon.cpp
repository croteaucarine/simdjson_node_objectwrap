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
    constructor.Value().Delete("jsObject");
    constructor.Value().Delete("keys");
    constructor.Value().Delete("arrayLength");
    constructor.Value().Delete("ToStringTag");
    
    /*std::chrono::steady_clock::time_point startGetFile;
    std::chrono::steady_clock::time_point endGetFile;
    std::chrono::steady_clock::time_point startAlloc;
    std::chrono::steady_clock::time_point endAlloc;
    std::chrono::steady_clock::time_point startParse;
    std::chrono::steady_clock::time_point endParse;*/

    //std::chrono::steady_clock::time_point  start = std::chrono::steady_clock::now();
    
    if(info.Length() == 1)
    {
      
      const Napi::Value objectToLoad = info[0];
      dom::parser parser;

      if(objectToLoad.IsString()) {
        // String passed is considered as JSON Document
        parser.parse(padded_string(objectToLoad.ToString()));
        _document = std::move(parser.doc);
      } else if (objectToLoad.IsObject()) {
        Napi::Object obj = objectToLoad.As<Napi::Object>();

        if(obj.Has("path") && obj.Get("path").IsString()) {
          parser.load(obj.Get("path").ToString());
          _document = std::move(parser.doc);

        } else if(obj.Has("doc") && obj.Get("doc").IsString()) {
          parser.parse(padded_string(obj.Get("doc").ToString()));
          _document = std::move(parser.doc);
        } else {
          // Throw error : Object is missing parameter path or doc
        }
      }

      //startGetFile = std::chrono::steady_clock::now();

      
      //endGetFile = std::chrono::steady_clock::now();
      /*if (res != 0)
      {
        Napi::Error::New(env, "Error parsing:" + simdjson::error_message(res)).ThrowAsJavaScriptException();
      }*/
      
      /*else
      {
        Napi::Error::New(env, "Error allocating memory:").ThrowAsJavaScriptException();
      }(*/
    } else if(info.Length() > 1) 
    {
      //Todo:throw error
    }
    //auto end = std::chrono::steady_clock::now();
    /*std::cout << "Getting parameter time : "  << std::chrono::duration_cast<std::chrono::nanoseconds>(endGetFile - startGetFile).count() << std::endl;
    std::cout << "Allocation time : "  << std::chrono::duration_cast<std::chrono::nanoseconds>(endAlloc - startAlloc).count() << std::endl;
    std::cout << "Parse time : "  << std::chrono::duration_cast<std::chrono::nanoseconds>(endParse - startParse).count() << std::endl;
    std::cout << "Constructor time: "  << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << std::endl;*/
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
    dom::parser parser;
    // Todo gérer exceptions si le paramètre est incorrect
      if(objectToLoad.IsString()) {
        // String passed is considered as JSON Document
        _textToParse = padded_string(objectToLoad.ToString());
        
      } else if (objectToLoad.IsObject()) {
        Napi::Object obj = objectToLoad.As<Napi::Object>();

        if(obj.Has("path") && obj.Get("path").IsString()) {
          std::string path = obj.Get("path").ToString();
          std::ifstream file(path);
          _textToParse = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
          
        } else if(obj.Has("doc") && obj.Get("doc").IsString()) {
          _textToParse = padded_string(obj.Get("doc").ToString());
        } else {
          // Throw error : Object is missing parameter path or doc
        }
      }
      return 0; // todo return error if params are incorrect
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
    //std::chrono::steady_clock::time_point  start = std::chrono::steady_clock::now();

    // todo : check if a doc is loaded. 

    dom::parser parser;
    parser.parse(_textToParse);
    _document = std::move(parser.doc);

    //std::chrono::steady_clock::time_point  end = std::chrono::steady_clock::now();

    //std::cout << "c++ Parse time : "  << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << std::endl;
    

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
      parsedJSONToString = "\n" + ParsedJSONToString(_document.root(), "");
      constructor.Value().Set("ToStringTag", Napi::String::From(env, parsedJSONToString));
    }

    return constructor.Value().Get("ToStringTag");;
  }

  //The JSON.parse() method parses a JSON string, constructing the JavaScript value or object described by the string. An optional reviver function can be provided to perform a transformation on the resulting object before it is returned.
  std::string ParsedJSONToString(dom::element element, std::string spaces, bool breakline = false)
  {
    int i = 0;
    std::string parsedJSONToString = "";

    switch (element.type()) {
      case dom::element_type::ARRAY:
        parsedJSONToString += "[";
        for (dom::element child : dom::array(element)) {
          if(i != 0) {
          parsedJSONToString += ", ";
        }
          parsedJSONToString += ParsedJSONToString(child, spaces, true); // let us recurse
          i++;
        }
        parsedJSONToString += "]"; 
        break;       
    case dom::element_type::OBJECT:
      // Todo: check spaces
      spaces  = "  " + spaces;
      if(breakline) {
        parsedJSONToString += "\n" + spaces;
      }
      
      parsedJSONToString += "{ \n";
      for (dom::key_value_pair field : dom::object(element)) {
        if(i != 0) {
          parsedJSONToString += ", \n";
        }
        parsedJSONToString += spaces + "  ";
        parsedJSONToString += field.key;
        parsedJSONToString += " : ";
        parsedJSONToString += ParsedJSONToString(field.value, "  " + spaces, false);
        i++;
      }
      parsedJSONToString += " \n" + spaces + "}";
      break;
    case dom::element_type::INT64:
      parsedJSONToString += std::to_string(int64_t(element));
      break;
    case dom::element_type::UINT64:
      parsedJSONToString += std::to_string(uint64_t(element));
      break;
    case dom::element_type::DOUBLE:
      parsedJSONToString += std::to_string(double(element));
      break;
    case dom::element_type::STRING:
      parsedJSONToString +=  std::string_view(element);
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
    dom::element element = _document.root();

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

  //Length
  Napi::Value Length(const Napi::CallbackInfo &info)
  {
    Napi::Env env = info.Env();
    if (!constructor.Value().Has("arrayLength")) {
      dom::element element = _document.root();
      { // calculate only if not already calculated to improve performances on multiple calls
        if (element.type() == dom::element_type::ARRAY)
        {
          int i = 0;
          /*for (dom::element child : dom::array(element)) {
            i++;
          }*/
          dom::array arrayElements = dom::array(element);
          dom::array::iterator it = arrayElements.begin();
          while(it != arrayElements.end()) {
              i++;
              it.operator++();
          }

          // Todo optimiser lengtH (https://github.com/simdjson/simdjson/pull/690/commits/de8950014b9def4cbf5dd80705153bdf56e94ba4)
          constructor.Value().Set("arrayLength", Napi::Number::From(env, i));
        }
        else
        {
          constructor.Value().Set("arrayLength", env.Undefined());
        }
      }
    }
    return constructor.Value().Get("arrayLength");
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
        return makeJSONObject1Depth(env, _document.root().at(jsonPointer));
      } catch (const std::exception& e) {
        Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
        return Napi::Value(env, env.Null()); 
      }
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
      std::string parsedJSONToString = "";
      Napi::Array jsArray;
      dom::element element = _document.root();
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
          constructor.Value().Set("arrayLength", i);       // Profiter de l'opportunité pour définir la propriété length
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
    dom::element element = _document.root();

    if (element.type() == dom::element_type::ARRAY)
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
  simdjson::padded_string _textToParse;
  dom::document _document;
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