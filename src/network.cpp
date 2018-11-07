#include "network.h"
#include "random.h"
#include <algorithm>
 
//----------------------------------------------------------------------
void Network::resize(const size_t &n) 
{
	values.resize(n);
	RNG.normal(values);
}
//----------------------------------------------------------------------

bool Network::add_link(const size_t& a, const size_t& b) 
{
	if (a!=b and a<size() and b<size()) {	
		auto range = links.equal_range(a);
		for (auto i = range.first; i != range.second; ++i) {
			if (i->second==b) {
				return false;
				}
		}
		links.insert({a,b});
		links.insert({b,a});
		return true;
	}
	return false;
}
//----------------------------------------------------------------------

size_t Network::random_connect(const double& mean_deg) 
{
	links.clear();
	
	size_t counter(0);
	
	for (size_t i(0); i<size(); ++i) {
		unsigned int deg(RNG.poisson(mean_deg));
		while (deg>(size()-1)){
			deg =(RNG.poisson(mean_deg));
		}
		std::vector<int> nodes (deg);
		RNG.uniform_int(nodes, 0, size()-1);
		for (size_t n(0); n<deg; ++n) {
			if(add_link(i, nodes[n])){
				++counter;
			}
		}
	}	
	return counter;
}
//----------------------------------------------------------------------

size_t Network::set_values(const std::vector<double>& newvalues) 
{
	unsigned int n(newvalues.size());
	size_t counter(0);
	
	if (n>size()) { n=size(); }
	
	for(unsigned int i=0; i<n; ++i) {
		values[i]=newvalues[i];
		++ counter;
	}
	return counter;
}
//----------------------------------------------------------------------

size_t Network::size() const 
{
	return values.size();
}
//----------------------------------------------------------------------

size_t Network::degree(const size_t &n) const 
{
	return links.count(n); 												//neighbors(n).size()
}
//----------------------------------------------------------------------

double Network::value(const size_t &n) const 
{
	return values[n];
}
//----------------------------------------------------------------------

std::vector<double> Network::sorted_values() const 
{
	std::vector<double> s_values (values);
	std::sort(s_values.rbegin(),s_values.rend());
	return s_values;
}
//----------------------------------------------------------------------

std::vector<size_t> Network::neighbors(const size_t& n) const 
{
	std::vector<size_t> _neighbors (0);
	if (n<size()) {
		auto range = links.equal_range(n);
		for (auto i = range.first; i != range.second; ++i) {
			_neighbors.push_back(i->second);
		}
	}
	return _neighbors;
}
//----------------------------------------------------------------------

/*size_t Network::random_connect(const double& mean_deg) 
{
	links.clear();
	
	size_t counter(0);

	std::vector<size_t> nodes(0);
	for (size_t j=0; j<size(); ++j) {
		nodes.push_back(j);						/// nodes est la liste 0,1,2,...,(N-1) (ou N est la taille du réseau)
	}

	std::vector<int> degrees (size());
	RNG.poisson(degrees, mean_deg);				///degrees[n] contient le nombre de links total du node n (qui suit une distribution de poisson)
	
	for (size_t i(0); i<size(); ++i) {									//on itère sur les indices i des noeuds
		RNG.shuffle(nodes);
		for (size_t n(0); n<nodes.size(); ++n) {						//on itère sur les indices des possibles liens
			if(degree(i)<degrees[i]) {									//on vérifie que le noeud i doit encore former des links
				if (degree(nodes[n])<degrees[nodes[n]]) {				//on vérifie que le noeud n est disponible pour un link
					if (add_link(i, nodes[n])) {
						++counter;										//on a crée un link
					}
				}
			} else {break;}
		}
	}	
	return counter;
}*/
