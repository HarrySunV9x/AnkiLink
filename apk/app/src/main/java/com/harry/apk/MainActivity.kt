package com.harry.apk

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.webkit.WebView
import android.widget.Button
import android.widget.LinearLayout
import android.widget.Toast
import androidx.constraintlayout.widget.ConstraintLayout
import com.harry.apk.databinding.ActivityMainBinding
import com.harry.apk.md.MarkdownParser
import java.io.File

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    /**
     * 当活动首次创建时调用。
     * 初始化活动的布局和视图组件。
     */
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        // 绑定视图布局
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        // 创建应用所需的私有目录
        createPrivateDirectory()

        // 创建并设置一个新按钮，用于触发 native 方法的调用
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
            // 设置按钮的点击监听器
            setOnClickListener {
                socketButtonFromJNI()
            }
        }
        // 将按钮添加到主布局中
        binding.root.addView(buttonInvokeNative)
    }

    /**
     * 调用由 native 库实现的方法。
     * 这个方法用于与 native 代码交互。
     */
    external fun socketButtonFromJNI(): Unit

    companion object {
        /**
         * 在应用启动时执行。
         * 用于加载 native 库。
         */
        init {
            System.loadLibrary("AnkiLink")
        }
    }

    /**
     * 创建一个私有目录来存储应用的文件。
     * 如果目录不存在，则创建它。
     */
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
                } else {
                    println("Create MDFiles Dir Failed")
                }
            } else {
                println("MDFiles Dir is existed: $privateDir")
            }
        } catch (e: Exception) {
            e.printStackTrace() // 输出异常信息
        }
    }

    /**
     * 从指定的 Markdown 文件加载内容，并显示在 WebView 中。
     * 如果文件不存在，则显示一个 Toast 通知。
     */
    fun flushMdDisplay() {
        val filePath = "/data/data/com.harry.apk/test.md"
        val file = File(filePath)

        if (!file.exists()) {
            Toast.makeText(this, "文件不存在", Toast.LENGTH_LONG).show()
            return
        }

        val htmlContent = MarkdownParser.parseFile(filePath)

        val htmlFile = File(filesDir, "tempHtml.html")
        htmlFile.writeText(htmlContent)

        val webView = WebView(this)
        webView.layoutParams = LinearLayout.LayoutParams(
            LinearLayout.LayoutParams.MATCH_PARENT,
            LinearLayout.LayoutParams.MATCH_PARENT
        )

        webView.loadDataWithBaseURL(null, htmlContent, "text/html", "UTF-8", null)
        setContentView(webView)
    }

    /**
     * 由 native 代码调用，用于在 socket 操作完成后更新 UI。
     * 这个方法确保 UI 更新在主线程上执行。
     */
    fun onSocketOperationComplete() {
        runOnUiThread {
            flushMdDisplay()
        }
    }

}
