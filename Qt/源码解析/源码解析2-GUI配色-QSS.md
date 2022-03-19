# Qt源码解析 [索引](https://blog.csdn.net/xinqingwuji/article/details/118365888)

# 源码解析2-GUI配色-QSS

## 复杂选择器示例

- 由于红色似乎是我们最喜欢的颜色，让我们通过设置以下应用程序范围的样式表将QLineEdit中的文本变为红色：

```css
QLineEdit { color: red }
```

- 但是，我们希望通过将 QLineEdit 显示为灰色来直观地指示QLineEdit是只读的：

```css
 QLineEdit { color: red }
 QLineEdit [ readOnly = "true" ] { color: gray }
```

- 在某些时候，我们的设计团队要求注册表中的所有QLineEdit （带有对象名称 registrationDialog）都是棕色的：

```scss
 QLineEdit { color: red }
 QLineEdit [ readOnly = "true" ] { color: gray }
 #registrationDialog QLineEdit { color: brown }
```

- 几次 UI 设计会议之后，我们决定所有的QDialog都应该是棕色的QLineEdit：

```css
 QLineEdit { color: red }
 QLineEdit [ readOnly = "true" ] { color: gray }
 QDialog  QLineEdit { color: brown }
```

- 测验：如果我们在QDialog中有一个只读的QLineEdit会发生什么？

​      提示：上面的冲突解决部分解释了在这种情况下会发生什么。具体查看冲突的处理。









