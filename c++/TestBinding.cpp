#include "TestBinding.h"

#include <dlfcn.h>
#include <jsi/JSIDynamic.h>
#include <testnum.h>
#include <chrono>  // std::chrono::seconds
#include <thread>

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

namespace example {

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
            auto str = TestCb();
            auto to_return = jsi::String::createFromAscii(runtime, str);
            free(str);
            eventhandler->callback.call(runtime, to_return);
          });
          t.detach();
          return jsi::Value::undefined();
        });
  }

  return jsi::Value::undefined();
}

}  // namespace example
