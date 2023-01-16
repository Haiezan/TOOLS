import jieba
from wordcloud import WordCloud
import matplotlib.pyplot as plt
from PIL import Image
import numpy as np

# 读取文本
f = open('20.txt','r',encoding = 'utf-8')
txt = f.read()
f.close

# 词频统计
words = jieba.lcut_for_search(txt)     #精确分词
counts = {}
for word in words:
    if len(word) == 1:
        continue
    else:
        counts[word] = counts.get(word,0) + 1

# 词云生成
mask = np.array(Image.open("mask.png")) # 底图
wordcloud = WordCloud(background_color="white",\
                      width = 800,\
                      height = 600,\
                      max_words = 2000,\
                      max_font_size = 150,\
                      mask = mask,\
                      contour_width = 3,\
                      contour_color = 'steelblue',\
                      font_path =  "造字工房映力黑规体.otf"
                      ).generate_from_frequencies(counts)
wordcloud.to_file('20.png')

# 显示图片
plt.imshow(wordcloud)
plt.axis("off")
plt.show()
