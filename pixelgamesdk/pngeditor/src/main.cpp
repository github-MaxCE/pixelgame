#define OLC_PGE_APPLICATION
#define OLC_PGEX_FUI
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <filesystem>
#include "olcPixelGameEngine.h"
#include "olcPGEX_FrostUI.h"
#include "stb_image_write.h"
#include "filemanager.h"

bool PointInsideRect(const olc::vi2d& point, const olc::vi2d& rectPos, const olc::vi2d& rectSize)
{
	return (point.x >= rectPos.x) &&
		(point.y >= rectPos.y) &&
		(point.x < rectPos.x + rectSize.x) &&
		(point.y < rectPos.y + rectSize.y);
}

class SpriteEditor : public olc::PixelGameEngine
{
	public:
		SpriteEditor()
		{
			sAppName = "Sprite Editor";
		}

	public: bool OnUserCreate() override
	{
		return true;
	}

		  olc::Sprite* sprite = nullptr;
		  olc::Pixel currentcolor = { 255, 0, 0 };
		  olc::vi2d s;
		  olc::vi2d offset;
		  int zoom = 1;
		  olc::FrostUI ui;

	public: bool OnUserUpdate(double dElapsedTime) override
	{
		Clear(olc::BLACK);

		if (GetKey(olc::NP_ADD).bPressed) zoom <<= 1;
		if (GetKey(olc::NP_SUB).bPressed) zoom >>= 1;
		zoom = zoom < 1 ? 1 : zoom;

		if (GetKey(olc::SHIFT).bHeld) offset.x += GetMouseWheel() / 5;
		else offset.y += GetMouseWheel() / 5;

		if (GetKey(olc::F1).bPressed && sprite != nullptr)
		{
			std::cout.setstate(std::ios_base::failbit);
			if (!ui.find_window("savesprite"))
			{
				auto savesprite = ui.create_window("savesprite", "Save file", { 10, 10 }, { 300, 200 });
				ui.add_inputfield(savesprite->get_id(), "savefilename", "file name", { 10, 10 }, { 100, 20 });
				ui.add_button(savesprite->get_id(), "savefile", "Save", { 10, 40 }, { 100, 20 },
					[&] {
						auto filepath = "../" + matpath() + ui.find_element("savefilename")->get_inputfield_value() + ".png";
						std::filesystem::remove(filepath);
						stbi_write_png(filepath.c_str(), sprite->width, sprite->height, 4, sprite->GetData(), sprite->width * 4);
					});
			}
			else
			{
				ui.find_window("savesprite")->close_window(!ui.find_window("savesprite")->get_closed_state());
			}
			std::cout.clear();
		}

		if (GetKey(olc::F2).bPressed && sprite == nullptr)
		{
			std::cout.setstate(std::ios_base::failbit);
			if (!ui.find_window("loadsprite"))
			{
				auto loadsprite = ui.create_window("loadsprite", "Load file", { 10, 10 }, { 200, 200 });
				ui.add_inputfield(loadsprite->get_id(), "loadfilename", "file", { 40, 10 }, { 100, 20 });
				ui.add_button(loadsprite->get_id(), "loadsprite", "Load", { 40, 40 }, { 100, 20 },
					[&] {
						sprite = new olc::Sprite{ "../" + matpath() + ui.find_element("loadfilename")->get_inputfield_value() + ".png" };
						ui.find_window("loadsprite")->close_window(true);
					});
			}
			else
			{
				ui.find_window("loadsprite")->close_window(!ui.find_window("loadsprite")->get_closed_state());
			}
			std::cout.clear();
		}


		static int valr = currentcolor.r,
			valg = currentcolor.g,
			valb = currentcolor.b,
			vala = currentcolor.a;

		if (GetKey(olc::F3).bPressed)
		{
			std::cout.setstate(std::ios_base::failbit);
			if (!ui.find_window("colormenu"))
			{
				auto colormenu = ui.create_window("colormenu", "Choose Color", { 10, 10 }, { 200, 200 });
				ui.add_int_slider(colormenu->get_id(), "r", "r", olc::vi2d{ 40, 10 }, olc::vi2d{ 100, 20 }, olc::vi2d{ 0, 255 }, &valr);
				ui.add_int_slider(colormenu->get_id(), "g", "g", olc::vi2d{ 40, 40 }, olc::vi2d{ 100, 20 }, olc::vi2d{ 0, 255 }, &valg);
				ui.add_int_slider(colormenu->get_id(), "b", "b", olc::vi2d{ 40, 70 }, olc::vi2d{ 100, 20 }, olc::vi2d{ 0, 255 }, &valb);
				ui.add_int_slider(colormenu->get_id(), "a", "alpha", olc::vi2d{ 40, 100 }, olc::vi2d{ 100, 20 }, olc::vi2d{ 0, 255 }, &vala);
			}
			else
			{
				ui.find_window("colormenu")->close_window(!ui.find_window("colormenu")->get_closed_state());
			}
			std::cout.clear();
		}

		currentcolor.r = valr;
		currentcolor.g = valg;
		currentcolor.b = valb;
		currentcolor.a = vala;

		if (GetKey(olc::F4).bPressed && sprite == nullptr)
		{
			std::cout.setstate(std::ios_base::failbit);
			if (!ui.find_window("newsprite"))
			{
				auto newsprite = ui.create_window("newsprite", "New Sprite", { 10, 10 }, { 300, 200 });
				ui.add_inputfield(newsprite->get_id(), "width", "x", { 10, 10 }, { 100, 20 });
				ui.add_inputfield(newsprite->get_id(), "height", "y", { 160, 10 }, { 100, 20 });
				ui.add_button(newsprite->get_id(), "createbutton", "Create", { 10, 50 }, { 100, 20 },
					[&] {
						sprite = new olc::Sprite{ atoi(ui.find_element("width")->get_inputfield_value().c_str()), atoi(ui.find_element("height")->get_inputfield_value().c_str()) };
						ui.find_window("newsprite")->close_window(true);
					});
			}
			else
			{
				ui.find_window("newsprite")->close_window(!ui.find_window("newsprite")->get_closed_state());
			}
			std::cout.clear();
		}

		ui.run();

		if (sprite != nullptr)
		{
			s.x = sprite->width * zoom;
			s.y = sprite->height * zoom;
			DrawRect(
				((ScreenSize() / 2) - (s / 2)) - 1 + offset,
				s + 1,
				olc::WHITE
			);

			FillRect(
				(ScreenSize() / 2) - (s / 2) + offset,
				s,
				olc::GREY
			);
			SetPixelMode(olc::Pixel::ALPHA);
			DrawSprite(
				(ScreenSize() / 2) - (s / 2) + offset,
				sprite,
				zoom
			);
			SetPixelMode(olc::Pixel::NORMAL);

			if (PointInsideRect(GetMousePos(), (ScreenSize() / 2 - s / 2 + offset), s) && !ui.isanywindowopen())
			{
				FillRect(
					(ScreenSize() / 2 - s / 2) + (((GetMousePos() - (ScreenSize() / 2 - s / 2)) / zoom) * zoom),
					{ zoom, zoom },
					currentcolor
				);
				if (GetMouse(olc::Mouse::LEFT).bHeld)
					sprite->SetPixel(
						(GetMousePos() - (ScreenSize() / 2 - s / 2 + offset)) / zoom,
						currentcolor
					);
			}
		}

		DrawRect({ 0, 0 }, ScreenSize() - 1, currentcolor);
		DrawRect({ 1, 1 }, ScreenSize() - 3, olc::GREY);

		FillRect(
			{ 2, 2 },
			{ 338, 9 },
			olc::DARK_GREY
		);

		std::cout.setstate(std::ios_base::failbit);
		DrawString(3, 3, "F1 Save", ui.find_window("savesprite") ? (ui.find_window("savesprite")->get_closed_state() ? olc::WHITE : olc::VERY_DARK_GREY) : olc::WHITE);
		DrawString(59, 3, " - ");
		DrawString(83, 3, "F2 Load", ui.find_window("loadsprite") ? (ui.find_window("loadsprite")->get_closed_state() ? olc::WHITE : olc::VERY_DARK_GREY) : olc::WHITE);
		DrawString(139, 3, " - ");
		DrawString(163, 3, "F3 Color Menu", ui.find_window("colormenu") ? (ui.find_window("colormenu")->get_closed_state() ? olc::WHITE : olc::VERY_DARK_GREY) : olc::WHITE);
		DrawString(267, 3, " - ");
		DrawString(291, 3, "F4 new", ui.find_window("newsprite") ? (ui.find_window("newsprite")->get_closed_state() ? olc::WHITE : olc::VERY_DARK_GREY) : olc::WHITE);
		std::cout.clear();

		return true;
	}

	public: bool OnUserDestroy() override
	{
		if(sprite != nullptr)
			sprite->~Sprite();
		return true;
	}
};


int main()
{
	SpriteEditor sdk;
	if (sdk.Construct(600, 300, 2, 2, false, true))
		sdk.Start();
	return 0;
}