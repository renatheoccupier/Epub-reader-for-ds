#include "book.h"
#include "pugixml.h"
#include "unzip.h"
#include <map>

struct epub_entry
{
	pugi::xml_node node;
	string image;
	epub_entry() {}
	epub_entry(const pugi::xml_node& n) : node(n) {}
	epub_entry(const string& img) : image(img) {}
	bool isImage() const { return !image.empty(); }
};

class epub_book : public Book
{
public:
	epub_book(const string& filename) : Book(filename) {};
	//~epub_book();
private:
	void parse();
	void parag_str (int parag_num);
	bool load_image(const string& zip_path);
	vector<epub_entry> par_index;
	int parse_doc(const pugi::xml_node& node);
	int extract_par(const pugi::xml_node& node);
	u32 total_paragraths() {return par_index.size();}
	bool push_it;
	pugi::xml_document document;
	std::map<string, u32> chapter_targets;
	std::map<string, u32> anchor_targets;
	std::map<string, unz_file_pos> zip_index;
};
