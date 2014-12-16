#include <iostream>
#include <set>
#include <map>
#include <list>
#include <vector>

template <class T1,class T2>
std::ostream &operator<<(std::ostream &a, const std::pair<T1,T2> &b)
{
	return a<<"("<<b.first<<","<<b.second<<")";
}

template <class T,class L>
std::ostream &operator<<(std::ostream &a, const std::set<T,L> &b)
{
	a<<"(";
		// typename explicit kiírása
	for (typename std::set<T,L>::const_iterator csi=b.begin(); csi!=b.end(); csi++)
		a<<(csi==b.begin()?"":", ")<<*csi;
	return a<<")";
};

template <class T, class V, class L>
std::ostream &operator<<(std::ostream &a, const std::map<T,V,L> &b)
{
	a<<"[";
		// typename explicit kiírása
	for (typename std::map<T,V,L>::const_iterator csi=b.begin(); csi!=b.end(); csi++)
		a<<(csi==b.begin()?"":", ")<<(*csi).first<<" => "<<(*csi).second;
	return a<<"]";
};

template <class T, class V, class L>
std::ostream &operator<<(std::ostream &a, const std::multimap<T,V,L> &b)
{
	a<<"[";
		// typename explicit kiírása
	for (typename std::multimap<T,V,L>::const_iterator csi=b.begin(); csi!=b.end(); csi++)
		a<<(csi==b.begin()?"":", ")<<(*csi).first<<" => "<<(*csi).second;
	return a<<"]";
};

template <class T>
std::ostream &operator<<(std::ostream &a, const std::list<T> &b)
{
	a<<"(";
		// typename explicit kiírása
	for (typename std::list<T>::const_iterator cli=b.begin(); cli!=b.end(); cli++)
		a<<(cli==b.begin()?"":", ")<<*cli;
	return a<<")";
};

template <class T>
std::ostream &operator<<(std::ostream &a, const std::vector<T> &b)
{
	a<<"[";
		// typename explicit kiírása
	for (typename std::vector<T>::const_iterator cvi=b.begin(); cvi!=b.end(); cvi++)
		a<<(cvi==b.begin()?"":", ")<<*cvi;
	return a<<"]";
};

