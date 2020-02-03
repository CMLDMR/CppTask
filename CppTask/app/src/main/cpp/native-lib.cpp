#include <jni.h>



#include "Renderer.h"
#include "Model2D.h"


Renderer *mRenderer;





extern "C"
JNIEXPORT void JNICALL Java_com_example_cpptask_MainActivity_onSurfaceCreatedC
        (JNIEnv * env, jclass cls,jint width, jint height) {

    //TODO: width height düzeltilecek
    mRenderer = Renderer::instance(width,width);
    if ( !mRenderer->CreateProgram(glVertexShader, glFragmentShader) )
    {
        LOGE ("Could not create program");
        return;
    }
    mRenderer->initVertexShader();
    mRenderer->clearGL();

}


extern "C"
JNIEXPORT void JNICALL Java_com_example_cpptask_MainActivity_onSurfaceChangedC
        (JNIEnv * env, jclass cls, jint width, jint height) {



}


extern "C"
JNIEXPORT void JNICALL Java_com_example_cpptask_MainActivity_onDrawFrameC
        (JNIEnv * env, jclass cls ) {

    mRenderer->updateScene();

}








extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_cpptask_MainActivity_onCreateModel(JNIEnv *env, jclass clazz,
                                                        jfloatArray f_array) {
    // TODO: implement onUpdateVerticesC()

    jstring modelID;

    jboolean cop = false;
    jsize len =env->GetArrayLength(f_array);
    jfloat *f = env->GetFloatArrayElements(f_array,&cop);

    if( !cop )
    {
        modelID = env->NewStringUTF("Error Occured: No Array Transfered");
        return modelID;//jstring ("Error Log");
    }

    Model2D* mModel = new Model2D(mRenderer);

    mModel->appendPoinst(f,len);

    mModel->startTimer(10);


    {
        Model2D* mModel2 = new Model2D(mRenderer);

        mModel2->appendPoinst(f,len);

        mModel2->startTimer(10);
        LOGE("Model2 Name Test: %s ",mModel2->getModelID().c_str());

    }


    modelID = env->NewStringUTF(mModel->getModelID().c_str());
    LOGE("MOdel Name Test: %s ",mModel->getModelID().c_str());







    return modelID;

}