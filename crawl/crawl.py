from bs4 import BeautifulSoup
import os
import urllib.request
import pymysql
import hashlib

def db_connect():
	conn = pymysql.connect(host='127.0.0.1', user='news', passwd='policies', db='news', charset='utf8', autocommit=True)
	return conn

def crawler(conn):
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
			parsing(conn, lst)
	return lst

def parsing(conn, lst):
	cur = conn.cursor()

	for u in lst:
		tf = 0
		a = urllib.request.urlopen(u['href'])
		encoding = a.headers.get_content_charset()
		txt = a.read().decode(encoding)
		article = BeautifulSoup(txt, 'html.parser')
		title = article.title
		print ("title: ", title)

		if article.find(property="me2:category1"):
			press = str(article.find(property="me2:category1")['content'])
		elif article.find("meta", {"name": "Author"}):
			press = str(article.find("meta", {"name": "Author"})['content'])
		elif article.find("address").find("a").get_text():
			press = str(article.find("address").find("a").get_text())
		print ("press: ", press)

		if article.find("div", {"class": "article_btns"}):
			href = article.find("div", {"class": "article_btns"}).find('a')['href']
		elif article.find("link", {"rel": "canonical"}):
			href = article.find("link", {"rel": "canonical"})['href']
		elif article.find("div", {"class": "article_info"}).find("a")['href']:
			href = article.find("div", {"class": "article_info"}).find("a")['href']
		print ("href: ", href)

#		contents = str(article.find(id="articleBodyContents").get_text()).replace("<br>", "").replace("</br>", "").replace("  ", " ")
#		contents.replace("\n", "")
#		h = hashlib.sha256(contents.encode(encoding)).hexdigest()

#		contents_lst = contents.split(" ")				

#		for tk in contents:
#			if b'\xbc\xbc\xbf\xf9\xc8\xa3'.decode("euc-kr") in tk:
#				tf = tf + 1

		ti = title.string.split(":")[0]
		#print (ti + " (" + press + "): " + str(href) + " - " + str(tf))
		ti = ti.replace("\"", "\\\"")
		ti = ti.replace("\'", "\\\'")
		#query = "insert into contents(keyword, title, press, url, hash) values('a', 'a', 'a', 'http://test.com', SHA1('a'))"
		query = "insert into contents(keyword, title, press, url) values('" + b'\xbc\xbc\xbf\xf9\xc8\xa3'.decode("euc-kr") +"', '" + ti +"', '" + press + "', '" + str(href) + "');";
		print ("query: ", query)
		cur.execute(query)
	cur.close()

def main():
	conn = db_connect()
	lst = crawler(conn)
	#parsing(conn, lst)
	conn.close()

if __name__ == "__main__":
	main()
