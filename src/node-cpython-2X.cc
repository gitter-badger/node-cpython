#include "node-cpython-2X.h"

extern "C" {
  #include <Python.h>


  // int run(char *filename, int arrc, char *arrv[]) {
  //
  //   Py_Initialize();
  //   PySys_SetArgvEx(arrc, arrv, 0);
  //
  //   // Get a reference to the main module.
  //   PyObject* main_module = PyImport_AddModule("__main__");
  //
  //   // Get the main module's dictionary
  //   // and make a copy of it.
  //   PyObject* main_dict = PyModule_GetDict(main_module);
  //   PyObject* main_dict_copy = PyDict_Copy(main_dict);
  //
  //   // Execute two different files of
  //   // Python code in separate environments
  //   FILE* file = fopen("/Users/rob/Desktop/node-cpython/example/multiply_2.py", "r");
  //   PyRun_File(file, filename, Py_file_input, main_dict, main_dict);
  //   PyRun_SimpleString("import sys\n");
  //
  //   Py_Finalize();
  //
  //   return 0;
  // }

}

Nan::Persistent<v8::Function> NodeCPython2X::constructor;

NodeCPython2X::NodeCPython2X() : main() {
}

NodeCPython2X::~NodeCPython2X() {
}

void NodeCPython2X::Init(v8::Local<v8::Object> exports) {
  Nan::HandleScope scope;

  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("NodeCPython2X").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  Nan::SetPrototypeMethod(tpl, "value", GetValue);

  Nan::SetPrototypeMethod(tpl, "preInit", PreInit);
  Nan::SetPrototypeMethod(tpl, "initialize", Initialize);
  Nan::SetPrototypeMethod(tpl, "finalize", Finalize);
  Nan::SetPrototypeMethod(tpl, "setPath", SetPath);

  Nan::SetPrototypeMethod(tpl, "addModule", AddModule);
  Nan::SetPrototypeMethod(tpl, "getDict", GetDict);

  Nan::SetPrototypeMethod(tpl, "simpleString", SimpleString);
  Nan::SetPrototypeMethod(tpl, "runString", RunString);

  constructor.Reset(tpl->GetFunction());
  exports->Set(Nan::New("NodeCPython2X").ToLocalChecked(), tpl->GetFunction());
}

void NodeCPython2X::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  if (info.IsConstructCall()) {
    // Invoked as constructor: `new NodeCPython2X(...)`
    NodeCPython2X* obj = new NodeCPython2X();
    obj->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  } else {
    // Invoked as plain function `NodeCPython2X(...)`, turn into construct call.
    const int argc = 1;
    v8::Local<v8::Value> argv[argc] = { info[0] };
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    info.GetReturnValue().Set(cons->NewInstance(argc, argv));
  }
}

void NodeCPython2X::GetValue(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  // NodeCPython2X* obj = ObjectWrap::Unwrap<NodeCPython2X>(info.Holder());
  // info.GetReturnValue().Set(Nan::New(obj->value_));
}


void NodeCPython2X::PreInit(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  // NodeCPython2X* obj = ObjectWrap::Unwrap<NodeCPython2X>(info.Holder());
  // obj->value_ += 1;
  // program = Py_DecodeLocale(info[0], NULL);
  // if (program == NULL) {
  //     fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
  //     exit(1);
  // }
  // obj->value_ = program;
  // info.GetReturnValue().Set(Nan::New(obj->program));
  // info.GetReturnValue().Set(Nan::New(obj->value_));
}

void NodeCPython2X::Initialize(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  // NodeCPython2X* obj = ObjectWrap::Unwrap<NodeCPython2X>(info.Holder());
  // obj->value_ += 1;
  // Py_SetProgramName(obj->program);  /* optional but recommended */
  // Py_Initialize();
  // info.GetReturnValue().Set(Nan::New(obj->value_));
  Py_Initialize();
}

void NodeCPython2X::Finalize(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  // NodeCPython2X* obj = ObjectWrap::Unwrap<NodeCPython2X>(info.Holder());
  // obj->value_ += 1;
  // Py_SetProgramName(program);  /* optional but recommended */
  // Py_Initialize();
  // PyMem_RawFree(obj->program);
  // info.GetReturnValue().Set(Nan::New(obj->value_));
  Py_Finalize();
}

void NodeCPython2X::SetPath(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  std::string argStr = std::string(*Nan::Utf8String(info[0]->ToString()));
  const char *str = argStr.c_str();
  // Lorenzo did some magic below
  PySys_SetPath(const_cast<char*>(str));
}

void NodeCPython2X::AddModule(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  NodeCPython2X* obj = ObjectWrap::Unwrap<NodeCPython2X>(info.Holder());

  obj->main = PyImport_AddModule("__main__");
  info.GetReturnValue().Set(Nan::New(obj->main));
}

void NodeCPython2X::GetDict(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  NodeCPython2X* obj = ObjectWrap::Unwrap<NodeCPython2X>(info.Holder());

  obj->d = PyModule_GetDict(obj->main);
  info.GetReturnValue().Set(Nan::New(obj->d));
}

// =============================================================================
// ================================== High Level ===============================
// =============================================================================

void NodeCPython2X::SimpleString(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  std::string pyStr = std::string(*Nan::Utf8String(info[0]->ToString()));
  const char *str = pyStr.c_str();
  PyRun_SimpleString(str);
}

void NodeCPython2X::RunString(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  NodeCPython2X* obj = ObjectWrap::Unwrap<NodeCPython2X>(info.Holder());

  std::string pyStr = std::string(*Nan::Utf8String(info[0]->ToString()));
  const char *str = pyStr.c_str();

  PyRun_String(str, Py_single_input, obj->d, obj->d);
}


// NAN_METHOD(simpleFile) {
//   NanScope();
//
//   // TODO: Check whether this check is necessary and
//   // if not redundant to JS check
//   // if (args.Length() != 1) {
//   //   NanThrowTypeError("Function expects one argument");
//   //   NanReturnUndefined();
//   // }
//
//   // v8::String::Utf8Value py_filepath_string_param(args[0]->ToString());
//   // std::string param0 = std::string(*py_filepath_string_param);
//   // const char *py_filepath_cstr = param0.c_str();
//   //
//   // v8::String::Utf8Value py_filename_string_param(args[1]->ToString());
//   // std::string param1 = std::string(*py_filename_string_param);
//   // const char *py_filename_cstr = param1.c_str();
//
//   FILE * fp;
//   fp = fopen(py_filepath_cstr, "r");
//
//   simple_File(fp, py_filename_cstr);
//
//   // TODO: Clean-up
//   NanReturnValue(NanNew(0));
// }
//
// NAN_METHOD(run) {
//   NanScope();
//
//   if (args.Length() != 3) {
//     NanThrowTypeError("Function expects one argument");
//     NanReturnUndefined();
//   }
//
//
//
//   v8::String::Utf8Value py_program_path_string(args[0]->ToString());
//   std::string param0 = std::string(*py_program_path_string);
//   const char *path = param0.c_str();
//
//   char *program_path;
//   strcpy(program_path, path);
//
//   printf("hello %s\n", program_path);
//
//   int arrc = args[1]->NumberValue();
//
//   Local<Array> arr= Local<Array>::Cast(args[2]);
//   char * arrv[] = {};
//
//   for (size_t i = 0; i < arrc; i++) {
//
//     Local<Value> item = arr->Get(i);
//     v8::String::Utf8Value array_string(item->ToString());
//     std::string param = std::string(*array_string);
//
//     // first cast const char to char and then
//     arrv[i] = strdup((char *) param.c_str());
//   }
//   // last statement: http://stackoverflow.com/a/1788749/3580261
//
//
//
//   run(program_path , arrc , arrv);
//
//   // TODO: Clean-up
//   NanReturnValue(NanNew(0));
// }
//
// NAN_METHOD(setArgv) {
//   NanScope();
//
//   // v8::String::Utf8Value py_filename_string_param(args[0]->ToString());
//   // std::string param1 = std::string(*py_filename_string_param);
//   // const char *py_filename_cstr = param1.c_str();
//
//   char *arrv[] = {"program name", "arg1", "arg2"};
//   int arrc = sizeof(arrv) / sizeof(char*) - 1;
//
//   pysetargv(arrc,arrv);
//
//   // TODO: Clean-up
//   NanReturnValue(NanNew(0));
// }
//
// NAN_METHOD(setProgramName) {
//   NanScope();
//
//   v8::String::Utf8Value py_program_name_string(args[0]->ToString());
//   std::string param0 = std::string(*py_program_name_string);
//   const char *py_program_name_cstr = param0.c_str();
//
//   setprogramname(py_program_name_cstr);
//
//   // TODO: Clean-up
//   NanReturnValue(NanNew(0));
// }
//
