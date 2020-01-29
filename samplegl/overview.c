#include "nuklear.h"
#include "nuklear_glfw_gl3.h"

static int
overview(struct nk_context *ctx)
{
	/* window flags */
	static int show_menu = nk_true;
	static int titlebar = nk_true;
	static int border = nk_true;
	static int resize = nk_true;
	static int movable = nk_true;
	static int no_scrollbar = nk_false;
	static int scale_left = nk_false;
	static nk_flags window_flags = 0;
	static int minimizable = nk_true;

	/* popups */
	static enum nk_style_header_align header_align = NK_HEADER_RIGHT;
	static int show_app_about = nk_false;

	/* window flags */
	window_flags = 0;
	ctx->style.window.header.align = header_align;
	if (border) window_flags |= NK_WINDOW_BORDER;
	if (resize) window_flags |= NK_WINDOW_SCALABLE;
	if (movable) window_flags |= NK_WINDOW_MOVABLE;
	if (no_scrollbar) window_flags |= NK_WINDOW_NO_SCROLLBAR;
	if (scale_left) window_flags |= NK_WINDOW_SCALE_LEFT;
	if (minimizable) window_flags |= NK_WINDOW_MINIMIZABLE;

	if (nk_begin(ctx, "Overview", nk_rect(10, 10, 400, 600), window_flags))
	{
		if (show_menu)
		{
			/* menubar */
			enum menu_states { MENU_DEFAULT, MENU_WINDOWS };
			static nk_size mprog = 60;
			static int mslider = 10;
			static int mcheck = nk_true;
			nk_menubar_begin(ctx);

			/* menu #1 */
			nk_layout_row_begin(ctx, NK_STATIC, 25, 5);
			nk_layout_row_push(ctx, 45);
			if (nk_menu_begin_label(ctx, "MENU", NK_TEXT_LEFT, nk_vec2(120, 200)))
			{
				static size_t prog = 40;
				static int slider = 10;
				static int check = nk_true;
				nk_layout_row_dynamic(ctx, 25, 1);
				if (nk_menu_item_label(ctx, "Hide", NK_TEXT_LEFT))
					show_menu = nk_false;
				if (nk_menu_item_label(ctx, "About", NK_TEXT_LEFT))
					show_app_about = nk_true;
				nk_progress(ctx, &prog, 100, NK_MODIFIABLE);
				nk_slider_int(ctx, 0, &slider, 16, 1);
				nk_checkbox_label(ctx, "check", &check);
				nk_menu_end(ctx);
			}
			/* menu #2 */
			nk_layout_row_push(ctx, 60);
			if (nk_menu_begin_label(ctx, "ADVANCED", NK_TEXT_LEFT, nk_vec2(200, 600)))
			{
				enum menu_state { MENU_NONE, MENU_FILE, MENU_EDIT, MENU_VIEW, MENU_CHART };
				static enum menu_state menu_state = MENU_NONE;
				enum nk_collapse_states state;

				state = (menu_state == MENU_FILE) ? NK_MAXIMIZED : NK_MINIMIZED;
				if (nk_tree_state_push(ctx, NK_TREE_TAB, "FILE", &state)) {
					menu_state = MENU_FILE;
					nk_menu_item_label(ctx, "New", NK_TEXT_LEFT);
					nk_menu_item_label(ctx, "Open", NK_TEXT_LEFT);
					nk_menu_item_label(ctx, "Save", NK_TEXT_LEFT);
					nk_menu_item_label(ctx, "Close", NK_TEXT_LEFT);
					nk_menu_item_label(ctx, "Exit", NK_TEXT_LEFT);
					nk_tree_pop(ctx);
				}
				else menu_state = (menu_state == MENU_FILE) ? MENU_NONE : menu_state;

				state = (menu_state == MENU_EDIT) ? NK_MAXIMIZED : NK_MINIMIZED;
				if (nk_tree_state_push(ctx, NK_TREE_TAB, "EDIT", &state)) {
					menu_state = MENU_EDIT;
					nk_menu_item_label(ctx, "Copy", NK_TEXT_LEFT);
					nk_menu_item_label(ctx, "Delete", NK_TEXT_LEFT);
					nk_menu_item_label(ctx, "Cut", NK_TEXT_LEFT);
					nk_menu_item_label(ctx, "Paste", NK_TEXT_LEFT);
					nk_tree_pop(ctx);
				}
				else menu_state = (menu_state == MENU_EDIT) ? MENU_NONE : menu_state;

				state = (menu_state == MENU_VIEW) ? NK_MAXIMIZED : NK_MINIMIZED;
				if (nk_tree_state_push(ctx, NK_TREE_TAB, "VIEW", &state)) {
					menu_state = MENU_VIEW;
					nk_menu_item_label(ctx, "About", NK_TEXT_LEFT);
					nk_menu_item_label(ctx, "Options", NK_TEXT_LEFT);
					nk_menu_item_label(ctx, "Customize", NK_TEXT_LEFT);
					nk_tree_pop(ctx);
				}
				else menu_state = (menu_state == MENU_VIEW) ? MENU_NONE : menu_state;

				state = (menu_state == MENU_CHART) ? NK_MAXIMIZED : NK_MINIMIZED;
				if (nk_tree_state_push(ctx, NK_TREE_TAB, "CHART", &state)) {
					size_t i = 0;
					const float values[] = { 26.0f,13.0f,30.0f,15.0f,25.0f,10.0f,20.0f,40.0f,12.0f,8.0f,22.0f,28.0f };
					menu_state = MENU_CHART;
					nk_layout_row_dynamic(ctx, 150, 1);
					nk_chart_begin(ctx, NK_CHART_COLUMN, NK_LEN(values), 0, 50);
					for (i = 0; i < NK_LEN(values); ++i)
						nk_chart_push(ctx, values[i]);
					nk_chart_end(ctx);
					nk_tree_pop(ctx);
				}
				else menu_state = (menu_state == MENU_CHART) ? MENU_NONE : menu_state;
				nk_menu_end(ctx);
			}
			/* menu widgets */
			nk_layout_row_push(ctx, 70);
			nk_progress(ctx, &mprog, 100, NK_MODIFIABLE);
			nk_slider_int(ctx, 0, &mslider, 16, 1);
			nk_checkbox_label(ctx, "check", &mcheck);
			nk_menubar_end(ctx);
		}

		if (show_app_about)
		{
			/* about popup */
			static struct nk_rect s = { 20, 100, 300, 190 };
			if (nk_popup_begin(ctx, NK_POPUP_STATIC, "About", NK_WINDOW_CLOSABLE, s))
			{
				nk_layout_row_dynamic(ctx, 20, 1);
				nk_label(ctx, "Nuklear", NK_TEXT_LEFT);
				nk_label(ctx, "By Micha Mettke", NK_TEXT_LEFT);
				nk_label(ctx, "nuklear is licensed under the public domain License.", NK_TEXT_LEFT);
				nk_popup_end(ctx);
			}
			else show_app_about = nk_false;
		}
	}
	nk_end(ctx);
	return !nk_window_is_closed(ctx, "Overview");
}
