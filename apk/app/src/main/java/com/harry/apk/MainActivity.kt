package com.harry.apk

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.Button
import androidx.constraintlayout.widget.ConstraintLayout
import com.harry.apk.databinding.ActivityMainBinding
import java.io.File

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        createPrivateDirectory()

        // 创建一个新的按钮
        val buttonInvokeNative = Button(this).apply {
            text = "Receive File Start"
            layoutParams = ConstraintLayout.LayoutParams(
                ConstraintLayout.LayoutParams.WRAP_CONTENT,
                ConstraintLayout.LayoutParams.WRAP_CONTENT
            ).apply {
                startToStart = ConstraintLayout.LayoutParams.PARENT_ID
                endToEnd = ConstraintLayout.LayoutParams.PARENT_ID
                topToTop = ConstraintLayout.LayoutParams.PARENT_ID
                bottomToBottom = ConstraintLayout.LayoutParams.PARENT_ID
            }
            setOnClickListener {
                socketButtonFromJNI()
            }
        }
        // 将按钮添加到布局中
        binding.root.addView(buttonInvokeNative)
    }

    /**
     * A native method that is implemented by the 'apk' native library,
     * which is packaged with this application.
     */
    external fun socketButtonFromJNI(): Unit;

    companion object {
        // Used to load the 'apk' library on application startup.
        init {
            System.loadLibrary("AnkiLink")
        }
    }

    private fun createPrivateDirectory() {
        try {
            // 获取私有目录路径
            val privateDir = filesDir

            // 创建新的目录，如果它不存在
            val newDir = File(privateDir, "mdfiles")
            if (!newDir.exists()) {
                val created = newDir.mkdir()
                if (created) {
                    println("Create MDFiles Dir Success")
                    // 目录创建成功
                } else {
                    println("Create MDFiles Dir Success")
                    // 目录创建失败
                }
            }
            else{// 目录存在
                println("MDFiles Dir is existed: $privateDir")
            }
        } catch (e: Exception) {
            // 处理异常
        }
    }
}