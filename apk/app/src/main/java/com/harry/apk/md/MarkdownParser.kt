/**
 * 文件名: MarkdownParser.kt
 * 包名: com.harry.apk.md
 * 文件描述: 该文件包含MarkdownParser对象，用于将Markdown文件内容转换为HTML格式。
 * 创建者: Harry
 * 创建日期: 2023-12-22
 */

package com.harry.apk.md


import com.vladsch.flexmark.html.HtmlRenderer
import com.vladsch.flexmark.parser.Parser
import com.vladsch.flexmark.util.data.MutableDataSet
import java.io.File

/**
 * 单例对象MarkdownParser。
 * 用于读取Markdown文件并将其内容转换为HTML格式。
 */
object MarkdownParser {

    /**
     * 解析指定路径的Markdown文件，并将其内容转换为HTML格式。
     *
     * @param filePath Markdown文件的路径。
     * @return 转换后的HTML字符串。
     */
    fun parseFile(filePath: String): String {

        // 配置选项
        val options = MutableDataSet()

        // 创建解析器和渲染器
        val parser = Parser.builder(options).build()
        val renderer = HtmlRenderer.builder(options).build()


        // 读取Markdown文件的内容
        val fileContent = File(filePath).readText()

        return renderer.render(parser.parse(fileContent))
    }
}
