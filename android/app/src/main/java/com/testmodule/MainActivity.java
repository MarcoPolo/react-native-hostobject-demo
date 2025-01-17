package com.testmodule;

import com.facebook.react.ReactActivity;
import com.facebook.react.ReactInstanceManager;
import com.facebook.react.bridge.ReactContext;

public class MainActivity extends ReactActivity implements ReactInstanceManager.ReactInstanceEventListener {
    static {
//        System.loadLibrary("testnum"); // this loads the library when the class is loaded
        System.loadLibrary("test_module_jni"); // this loads the library when the class is loaded
    }

    /**
     * Returns the name of the main component registered from JavaScript.
     * This is used to schedule rendering of the component.
     */
    @Override
    protected String getMainComponentName() {
        return "TestModule";
    }

    @Override
    public void onResume() {
        super.onResume();
//        hi();
        getReactInstanceManager().addReactInstanceEventListener(this);
    }

    @Override
    public void onPause() {
        super.onPause();
        getReactInstanceManager().removeReactInstanceEventListener(this);
//        hi();
    }

    @Override
    public void onReactContextInitialized(ReactContext context) {
//        hi();
        install(context.getJavaScriptContextHolder().get());
    }

    public native void install(long jsContextNativePointer);
//    public native void hi();
}
