from bs4 import BeautifulSoup
import os
import urllib.request

def crawler():
	for i in range(1, 10):
		req = "http://news.naver.com/main/search/search.nhn?query=%BC%BC%BF%F9%C8%A3&ie=MS949&startDate=2016-12-01&endDate=2016-12-27&page=" + str(i)
		with urllib.request.urlopen(req) as f:
			doc = f.read()
			soup = BeautifulSoup(doc, 'html.parser')

			news = soup.find(id="search_div")
			url_lst = news.find_all('a')

			lst = []
			for u in url_lst:
				if "go_naver" in str(u):
					lst.append(u)

			for u in lst:
				tf = 0
				a = urllib.request.urlopen(u['href'])
				try:
					txt = a.read().decode("euc-kr")
				except:
					continue
				article = BeautifulSoup(txt, 'html.parser')
				title = article.title
				press = str(article.find(property="me2:category1")['content'])
				href = article.find("div", {"class": "article_btns"}).find('a')['href']
				contents = str(article.find(id="articleBodyContents")).replace("<br>", "").replace("</br>", "").replace("  ", " ").split(" ")

				for tk in contents:
					if "세월호" in tk:
						tf = tf + 1

				print (title.string.split(":")[0] + " (" + press + "): " + str(href) + " - " + str(tf))

def main():
	crawler()

if __name__ == "__main__":
	main()
