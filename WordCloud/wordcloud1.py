from wordcloud import WordCloud
import matplotlib.pyplot as plt
from PIL import Image
import numpy as np

# 底图
mask = np.array(Image.open("2022.png"))

# 数据 项目+频率
frequencies = {}
for line in open('Company.txt','r',encoding = 'utf-8'):
    arr = line.split(" ")
    frequencies[arr[0]] = float(arr[1])

#f = open('txt.txt','r',encoding = 'utf-8')
#txt = f.read()
#f.close

# 词云生成
wordcloud = WordCloud(background_color="white",\
                      width = 800,\
                      height = 600,\
                      max_words = 200,\
                      max_font_size = 80,\
                      mask = mask,\
                      contour_width = 3,\
                      contour_color = 'steelblue',\
                      font_path =  "造字工房映力黑规体.otf"
                      ).generate_from_frequencies(frequencies)
wordcloud.to_file('Company2022.png')

# 显示图片
plt.imshow(wordcloud)
plt.axis("off")
plt.show()