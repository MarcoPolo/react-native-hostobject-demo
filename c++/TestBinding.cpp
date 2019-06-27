#include "TestBinding.h"

#include <dlfcn.h>
#include <jsi/JSIDynamic.h>
#include <chrono>  // std::chrono::seconds
#include <thread>
#include "testnum.h"

struct EventHandlerWrapper {
  EventHandlerWrapper(jsi::Function eventHandler)
      : callback(std::move(eventHandler)) {}

  jsi::Function callback;
};

#if ANDROID
extern "C" {
JNIEXPORT void JNICALL Java_com_testmodule_MainActivity_install(
    JNIEnv *env, jobject thiz, jlong runtimePtr) {
  auto testBinding = std::make_shared<example::TestBinding>();
  jsi::Runtime *runtime = (jsi::Runtime *)runtimePtr;

  example::TestBinding::install(*runtime, testBinding);
}
}
#endif

// void callCb(jsi::Runtime &runtime, jsi::Function &fn) {
// void callCb(void (*)()) {
//   std::this_thread::sleep_for(std::chrono::seconds(1));
//   // fn.call(runtime, {});
//   fn();
// }

namespace example {

static jsi::Object getModule(jsi::Runtime &runtime,
                             const std::string &moduleName) {
  auto batchedBridge =
      runtime.global().getPropertyAsObject(runtime, "__fbBatchedBridge");
  auto getCallableModule =
      batchedBridge.getPropertyAsFunction(runtime, "getCallableModule");
  auto module =
      getCallableModule
          .callWithThis(runtime, batchedBridge,
                        {jsi::String::createFromUtf8(runtime, moduleName)})
          .asObject(runtime);
  return module;
}

void TestBinding::install(jsi::Runtime &runtime,
                          std::shared_ptr<TestBinding> testBinding) {
  auto testModuleName = "nativeTest";
  auto object = jsi::Object::createFromHostObject(runtime, testBinding);
  runtime.global().setProperty(runtime, testModuleName, std::move(object));
}

TestBinding::TestBinding() {}

jsi::Value TestBinding::get(jsi::Runtime &runtime,
                            const jsi::PropNameID &name) {
  auto methodName = name.utf8(runtime);

  if (methodName == "runTest") {
    return jsi::Function::createFromHostFunction(
        runtime, name, 0,
        [](jsi::Runtime &runtime, const jsi::Value &thisValue,
           const jsi::Value *arguments,
           size_t count) -> jsi::Value { return TestNum(); });
  }

  // if (methodName == "goTest") {
  //   return jsi::Function::createFromHostFunction(
  //       runtime, name, 0,
  //       [&test](jsi::Runtime &runtime, const jsi::Value &thisValue,
  //               const jsi::Value *arguments, size_t count) -> jsi::Value {
  //         char *error;
  //         auto handle = dlopen("libgojni.so", RTLD_LAZY);
  //         // auto handle = dlopen("libyoga.so", RTLD_LAZY);
  //         error = dlerror();
  //         if (error != NULL) {
  //           auto s = jsi::String::createFromAscii(runtime, error);
  //           return s;
  //         }
  //         return jsi::Value::undefined();

  //         auto TestNum = (int (*)(void))dlsym(handle, "TestNum");
  //         error = dlerror();
  //         if (error != NULL) {
  //           auto s = jsi::String::createFromAscii(runtime, error);
  //           return s;
  //           // jsi::Value v(runtime, s);
  //           // return v;
  //           // return jserror.value();
  //         } else {
  //           return TestNum();
  //         }
  //         return 10;
  //       });
  // }

  if (methodName == "runCb") {
    return jsi::Function::createFromHostFunction(
        runtime, name, 0,
        [](jsi::Runtime &runtime, const jsi::Value &thisValue,
           const jsi::Value *arguments, size_t count) -> jsi::Value {
          auto fn = arguments[0].getObject(runtime).asFunction(runtime);
          auto eventhandler =
              std::make_shared<EventHandlerWrapper>(std::move(fn));
          // fn.call(runtime, {});
          std::thread t([eventhandler, &runtime]() {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            eventhandler->callback.call(
                runtime, jsi::String::createFromAscii(runtime, "Hello again!"));
            eventhandler->callback.call(runtime, TestCb());
          });
          t.detach();
          return jsi::Value::undefined();
        });
  }

  return jsi::Value::undefined();
}

}  // namespace example
