#pragma once
#include "utf.h"

class TTreeView : public TNotifyWin, public THasIconWin {
private:
	void set_image_list(bool normal = true);
	virtual void clean_data(int) = 0;
	Handle get_next(Handle itm);
	Handle get_child(Handle itm);
	Handle get_prev(Handle itm);
	void clean_subitems(Handle itm);
	Handle ins_mode;
public:
	TTreeView(TEventWindow* form, bool has_lines = true);
	Handle add_item(pchar caption, Handle parent = NULL, int idx1 = 0, int idx2 = -1, int data = 0);
	void insert_mode(Handle mode);
	void insert_mode(const char* mode);
	int get_item_data(Handle pn);
	void select(Handle p);
	int load_icons(pchar path, bool small_size) override;
	Handle get_selected();
	const std::vector<int> iterate_item(Handle itm);
	void clear();
	gui_string get_item_text(Handle);
	void set_item_text(void* itm, pchar str);
	void set_foreground(COLORREF clr);
	void set_background(COLORREF clr);
	void makeLabelEditable(bool toBeEnabled);
	void set_theme(bool explorer);

	Handle get_root();
	void iterate_childs(Handle itm);
	void remove_item(Handle itm);

	virtual int handle_rclick() = 0;
	virtual void handle_select(Handle) = 0;
	virtual void handle_dbclick(Handle) = 0;
	int handle_notify(void* p) override;
};
