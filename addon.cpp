#include <napi.h>
#include "simdjson/src/simdjson.h"

Napi::ObjectReference testStaticContextRef;


class simdjsonJS : public Napi::ObjectWrap<simdjsonJS> {
public:
  simdjsonJS(const Napi::CallbackInfo& info) : Napi::ObjectWrap<simdjsonJS>(info) {
    Napi::Env env = info.Env();
    if(info.Length() == 0) {
      

      //finalizeCb_ = Napi::Persistent(info[0].As<Napi::Function>());
      Napi::Error::New(env, "Json path undefined").ThrowAsJavaScriptException();
      return;
    } else {
      // Todo gérer exceptions si le paramètre est incorrect
      Napi::Value param = info[0].As<Napi::Value>();
      this->_jsonRaw = param.ToString();
      
      bool alloc = _pj.allocate_capacity(this->_jsonRaw.size()); // allocate memory for parsing up to p.size() bytes
      if(alloc) {
        const int res = simdjson::json_parse(this->_jsonRaw, _pj); // do the parsing, return 0 on success
        // parsing is done!
        if (res != 0) {
          Napi::Error::New(env, "Error parsing:" + simdjson::error_message(res)).ThrowAsJavaScriptException();
        }
      } else {
          Napi::Error::New(env, "Error allocating memory:").ThrowAsJavaScriptException();
        }
    }
  }

  void Setter(const Napi::CallbackInfo& /*info*/, const Napi::Value& value) {
    _jsonRaw = value.ToString();
  }
  

  Napi::Value Getter(const Napi::CallbackInfo& info) {
    // Todo : Si pas de key, retourne le résultats simdjson en string

    // Si une key est passée en paramètre, envoyer seulement la branche demandée
    return Napi::String::New(info.Env(), _jsonRaw);
  }

  Napi::Value toJSON(const Napi::CallbackInfo& info) {
    
    return Napi::Number::New(info.Env(), 123);
  }

  Napi::Value ToStringTag(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    simdjson::ParsedJson::Iterator pjh(_pj);
    std::string parsedJSONToString = "";

    if (!pjh.is_ok()) {
       Napi::Error::New(env,  "Could not iterate parsed result. ").ThrowAsJavaScriptException();
    } else
    {
      parsedJSONToString = std::string("\n") + ParsedJSONToString(pjh, "");
    }

    return Napi::String::From(info.Env(), parsedJSONToString);
  }

  //The JSON.parse() method parses a JSON string, constructing the JavaScript value or object described by the string. An optional reviver function can be provided to perform a transformation on the resulting object before it is returned.
  std::string ParsedJSONToString(simdjson::ParsedJson::Iterator &pjh, std::string spaces) {
    std::string parsedJSONToString = "";
   if (pjh.is_object()) {
    parsedJSONToString += "{";
    if (pjh.down()) {
      //pjh.print(std::cout); // must be a string
      parsedJSONToString += spaces + pjh.get_string();
      parsedJSONToString += " : ";
      pjh.next();
      parsedJSONToString += ParsedJSONToString(pjh, spaces); // let us recurse
      while (pjh.next()) {
        parsedJSONToString += ", \n";
        parsedJSONToString += pjh.get_string();
        parsedJSONToString += ":";
        pjh.next();
        parsedJSONToString += ParsedJSONToString(pjh, spaces); // let us recurse
      }
      pjh.up();
    }
    parsedJSONToString += "}";
  } else if (pjh.is_array()) {
    parsedJSONToString += "[";
    if (pjh.down()) {
      parsedJSONToString += ParsedJSONToString(pjh, spaces); // let us recurse
      while (pjh.next()) {
        parsedJSONToString += ", ";
        parsedJSONToString += ParsedJSONToString(pjh, spaces); // let us recurse
      }
      pjh.up();
    }
    parsedJSONToString += "]";
  } else if (pjh.is_integer()) {
    
    parsedJSONToString += std::to_string(pjh.get_integer());
  } else if (pjh.is_double()) {
    parsedJSONToString += std::to_string(pjh.get_double());
  } else if (pjh.is_true()) {
      parsedJSONToString += "true";
  } else if (pjh.is_false()) {
      parsedJSONToString += "false";
  } else {
    parsedJSONToString += pjh.get_string(); // just print the lone value
  }

  return parsedJSONToString;
}

// Keys
Napi::Value Keys(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    simdjson::ParsedJson::Iterator pjh(_pj);
    std::string parsedJSONToString = "";
    Napi::Array array = Napi::Array::New(env);
    int i = 0;

    if (pjh.is_object()) {
    if (pjh.down()) {
        array.Set(i, Napi::String::From(info.Env(), pjh.get_string()));
        i++;
        pjh.next();
        while (pjh.next()) {
          array.Set(i, Napi::String::From(info.Env(), pjh.get_string()));
          i++;
        }
      }
    } else if(pjh.is_array()) {
      if (pjh.down()) {
        array.Set(i, Napi::String::From(info.Env(), std::to_string(i)));
        i++;
        while (pjh.next()) {
          array.Set(i, Napi::String::From(info.Env(), std::to_string(i)));
          i++;
        }
      }
    }

    return array;
  }

  //Length
  Napi::Value Length(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    simdjson::ParsedJson::Iterator pjh(_pj);
    std::string parsedJSONToString = "";
    int i = 0;
    
      if (pjh.is_array()) {
    
        if (pjh.down()) {
          i++;
          while (pjh.next()) {
            i++;
        }
      }
    }
    return Napi::Number::From(env, i);
  }

  // todo: implémenter les foncitons de tableau
  // Array.prototype.filter() : https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/filter
  // Array.prototype.forEach() : itérateur??
  // Array.isArray() : bool : vraiment facile
  // Array.prototype.every() : The every() method tests whether all elements in the array pass the test implemented by the provided function. It returns a Boolean value. 
  // Array.prototype.some() : The some() method tests whether at least one element in the array passes the test implemented by the provided function. It returns a Boolean value. 
  // Array.prototype.map() : The map() method creates a new array with the results of calling a provided function on every element in the calling array.
  // Array.prototype.reduce() : The reduce() method executes a reducer function (that you provide) on each element of the array, resulting in a single output value.
  // Array.prototype.find() : The find() method returns the value of the first element in the provided array that satisfies the provided testing function.
  // Array.sort?


  // toto : implémenter le parcours d'objets 
  // https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object
  // https://stackoverflow.com/questions/13964155/get-javascript-object-from-array-of-objects-by-value-of-property
  // Object.values()
  // Object.getOwnPropertyDescriptor() : La méthode Object.getOwnPropertyDescriptor() renvoie un descripteur de la propriété propre d'un objet (c'est-à-dire une propriété directement présente et pas héritée via la chaîne de prototypes).
  // La méthode Object.values() renvoie un tableau contenant les valeurs des propriétés propres énumérables d'un objet dont l'ordre est le même que celui obtenu avec une boucle for...in (la boucle for-in est différente car elle parcourt également les propriétés héritées).

 // Returns an array with JSON elements
 // Todo : Compare difference between 
  Napi::Value Iterator(const Napi::CallbackInfo& info) {
    Napi::Array array = Napi::Array::New(info.Env());

    Napi::Env env = info.Env();
    simdjson::ParsedJson::Iterator pjh(_pj);
    std::string parsedJSONToString = "";
    int i = 0;
    
      if (pjh.is_array()) {
        if (pjh.down()) {
          array.Set(i, makeJSONObject1Depth(info.Env(), pjh));
          i++;
          while (pjh.next()) {
            // todo retourner la valeur de l'objet ou (Object si objet complexe!!!)
            array.Set(i, makeJSONObject1Depth(info.Env(), pjh));
            i++;
          }
        }
        return array.Get(Napi::Symbol::WellKnown(info.Env(), "iterator")).As<Napi::Function>().Call(array, {});
      } else {
        // Throw error Object is not iterable
        Napi::Error::New(env, "Object is not iterable").ThrowAsJavaScriptException();
        return Napi::Value(info.Env(), env.Null()); // Return null value when error occurs. 
      }

      //array.Set(array.Length(), Napi::String::From(info.Env(), "Objet à définir"));
  }

  Napi::Value makeJSONObject(Napi::Env env, simdjson::ParsedJson::Iterator & pjh) {
  Napi::Value v;
  if (pjh.is_object()) {
    Napi::Object obj = Napi::Object::New(env); // {
    if (pjh.down()) {
      // must be a string
      Napi::String key = Napi::String::New(env, pjh.get_string());
      // :
      pjh.next();
      Napi::Value value = makeJSONObject(env, pjh); // let us recurse
      obj.Set(key, value);
      while (pjh.next()) { // ,
        key = Napi::String::New(env, pjh.get_string());
        pjh.next();
        // :
        value = makeJSONObject(env, pjh); // let us recurse
        obj.Set(key, value);
      }
      pjh.up();
    }
    v = obj; // }
  } else if (pjh.is_array()) {
    std::vector<Napi::Value> arr;
    if (pjh.down()) {
      // [
      Napi::Value value = makeJSONObject(env, pjh); // let us recurse
      arr.push_back(value);
      while (pjh.next()) { // ,
        value = makeJSONObject(env, pjh); // let us recurse
        arr.push_back(value);
      }
      pjh.up();
    }
    // ]
    Napi::Array array = Napi::Array::New(env, arr.size());
    for (std::size_t i{ 0 }; i < arr.size(); i++) array.Set(i, arr[i]);
    v = array;
  } else if (pjh.is_string()) {
    v = Napi::String::New(env, pjh.get_string());
  } else if (pjh.is_double()) {
    v = Napi::Number::New(env, pjh.get_double());
  } else if (pjh.is_integer()) {
    v = Napi::Number::New(env, pjh.get_integer());
  } else {
    switch (pjh.get_type()) {
      case 't':  {
        v = Napi::Boolean::New(env, true);
        break;
      }
      case 'f': {
        v = Napi::Boolean::New(env, false);
        break;
      }
      case 'n': {
        v = env.Null();
        break;
      }
      default : break;
    }
  }
  
  return v;
}

struct UserDataHolder {
  size_t location;
};

Napi::Value ObjectGetter(const Napi::CallbackInfo& info) {
   const UserDataHolder* holder = reinterpret_cast<UserDataHolder*>(info.Data());

   //std::cout << holder->location;
   //std::cout << "\n";
  simdjson::ParsedJson::Iterator pjh(_pj);
  pjh.move_to_specific_location(holder->location);
  // std::cout << holder->location;
  // std::cout << pjh.get_type();
  //   std::cout << "\n";
  // if(pjh.is_object()) {
  //   std::cout << "2. object";
  //   std::cout << "\n";
  // } else if (pjh.is_array()) {
  //   std::cout << "2. array";
  //   std::cout << "\n";
  // } 
  
  // return Napi::Value(info.Env(), info.Env().Null());  
  return makeJSONObject1Depth(info.Env(), pjh);
}

Napi::PropertyDescriptor getProperty(Napi::Env env, Napi::Object& obj, Napi::String key, simdjson::ParsedJson::Iterator& pjh) {
  Napi::PropertyDescriptor pd = Napi::PropertyDescriptor::Value("", env.Null()); 
  
  if(pjh.is_object()) {
      UserDataHolder* holder = new UserDataHolder();
      holder->location = pjh.get_tape_location();
      // std::cout << "1. object : ";
      // std::cout << key.ToString();
      // std::cout << " : ";
      // std::cout << holder->location;
      // std::cout << "\n";

      auto callbackGetter = std::bind(&simdjsonJS::ObjectGetter, this, std::placeholders::_1);
      pd = Napi::PropertyDescriptor::Accessor(env, obj, key, callbackGetter, napi_enumerable, reinterpret_cast<void*>(holder));
   } else if (pjh.is_array()) {
     UserDataHolder* holder = new UserDataHolder();
     holder->location = pjh.get_tape_location(); 

    //   std::cout << "1. array";
    //   std::cout << key;
    //   std::cout << " : ";
    //   std::cout << holder->location;
    //   std::cout << "\n";

    auto callbackGetter = std::bind(&simdjsonJS::ObjectGetter, this, std::placeholders::_1);
    pd = Napi::PropertyDescriptor::Accessor(env, obj, key, callbackGetter, napi_enumerable, reinterpret_cast<void*>(holder));

  } else if (pjh.is_string()) {
    pd = Napi::PropertyDescriptor::Value(key, Napi::String::New(env, pjh.get_string()), napi_enumerable);
  } else if (pjh.is_double()) {
    pd = Napi::PropertyDescriptor::Value(key, Napi::Number::New(env, pjh.get_double()), napi_enumerable);
  } else if (pjh.is_integer()) {
    pd = Napi::PropertyDescriptor::Value(key, Napi::Number::New(env, pjh.get_integer()), napi_enumerable);
  } else {
    switch (pjh.get_type()) {
      case 't':  {
        pd = Napi::PropertyDescriptor::Value(key, Napi::Boolean::New(env, true), napi_enumerable);
        break;
      }
      case 'f': {
        pd = Napi::PropertyDescriptor::Value(key, Napi::Boolean::New(env, false), napi_enumerable);
        break;
      }
      case 'n': {
        pd = Napi::PropertyDescriptor::Value(key, env.Null(), napi_enumerable);
        break;
      }
      default : break;
    }
  }

  return pd;
}

  Napi::Value makeJSONObject1Depth(Napi::Env env, simdjson::ParsedJson::Iterator & pjh) {
  Napi::Value v;

  if (pjh.is_object()) {
    Napi::Object obj = Napi::Object::New(env); // {
    if (pjh.down()) {
      // must be a string
      Napi::String key = Napi::String::New(env, pjh.get_string());
      // :
      pjh.next();
      obj.DefineProperty(getProperty(env, obj, key, pjh));
      while (pjh.next()) { // ,
        key = Napi::String::New(env, pjh.get_string());
        pjh.next();        
        obj.DefineProperty(getProperty(env, obj, key, pjh));
      }
      pjh.up();
    }
    v = obj; // }
  } else if (pjh.is_array()) {
    std::vector<Napi::Value> arr;
    if (pjh.down()) {
      // [
      Napi::Value value = makeJSONObject1Depth(env, pjh); // let us recurse
      arr.push_back(value);
      while (pjh.next()) { // ,
        value = makeJSONObject1Depth(env, pjh); // let us recurse
        arr.push_back(value);
      }
      pjh.up();
    }
    // ]
    Napi::Array array = Napi::Array::New(env, arr.size());
    for (std::size_t i{ 0 }; i < arr.size(); i++) array.Set(i, arr[i]);
    v = array;
  } else if (pjh.is_string()) {
    v = Napi::String::New(env, pjh.get_string());
  } else if (pjh.is_double()) {
    v = Napi::Number::New(env, pjh.get_double());
  } else if (pjh.is_integer()) {
    v = Napi::Number::New(env, pjh.get_integer());
  } else {
    switch (pjh.get_type()) {
      case 't':  {
        v = Napi::Boolean::New(env, true);
        break;
      }
      case 'f': {
        v = Napi::Boolean::New(env, false);
        break;
      }
      case 'n': {
        v = env.Null();
        break;
      }
      default : break;
    }
  }
  
  return v;
}

  static void Initialize(Napi::Env env, Napi::Object exports) {

    exports.Set("simdjson", DefineClass(env, "simdjson", {
      InstanceValue("value", Napi::Boolean::New(env, true), napi_enumerable),
      InstanceAccessor("get", &simdjsonJS::Getter, nullptr, napi_enumerable),
      InstanceAccessor("set", nullptr, &simdjsonJS::Setter, napi_enumerable),
      InstanceMethod("keys", &simdjsonJS::Keys, napi_enumerable),
      InstanceAccessor("length", &simdjsonJS::Length, nullptr, napi_enumerable),

      // conventions
      InstanceMethod("toJSON", &simdjsonJS::toJSON, napi_enumerable),
      InstanceAccessor(Napi::Symbol::WellKnown(env, "toStringTag"), &simdjsonJS::ToStringTag, nullptr, napi_enumerable),
      InstanceMethod(Napi::Symbol::WellKnown(env, "iterator"), &simdjsonJS::Iterator, napi_enumerable)

    }));
  }

  void Finalize(Napi::Env env) {

    /*if(finalizeCb_.IsEmpty()) {
      return;
    }

    finalizeCb_.Call(env.Global(), {Napi::Boolean::New(env, true)});
    finalizeCb_.Unref();*/

  }

private:
  std::string _jsonRaw;
  std::string _jsonString;
  simdjson::ParsedJson _pj;
};

// Initialize native add-on
Napi::Object Init (Napi::Env env, Napi::Object exports) {
    simdjsonJS::Initialize(env, exports);
    return exports;
}

// Register and initialize native add-on
NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)