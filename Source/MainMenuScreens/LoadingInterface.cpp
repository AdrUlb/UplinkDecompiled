#include <MainMenuScreens/LoadingInterface.hpp>

#include <Eclipse.hpp>
#include <Globals.hpp>
#include <Opengl.hpp>
#include <RedShirt.hpp>

void LoadingInterface::Create()
{
	if (IsVisible())
		return;

	const auto width = app->GetOptions().GetOptionValue("graphics_screenwidth");
	const auto height = app->GetOptions().GetOptionValue("graphics_screenheight");
	const auto filePath = app->GetOptions().ThemeFilename("loading/filenames.txt");

	FILE* file = RsArchiveFileOpen(filePath, "rt");

	UplinkAssert(file != nullptr);

	LList<char*> loadingImages;

	while (!feof(file))
	{
		const auto fileName = new char[0x100];
		fscanf(file, "%s\n", fileName);
		if (strcmp(fileName, "loading/kris.tif") == 0)
		{
			delete[] fileName;
			continue;
		}

		loadingImages.PutData(fileName);
	}

	const int imageIndex = (rand() / RAND_MAX) * loadingImages.Size();
	EclRegisterButton(0, 0, width, height, "", "", "loading_background");
	button_assignbitmap("loading_background", loadingImages[imageIndex]);
	EclGetButton("loading_background")->ImageNormal->Scale(512, 512);
	EclRegisterButtonCallbacks("loading_background", imagebutton_drawtextured, nullptr, nullptr, nullptr);

	DeleteLListData(&loadingImages);

	EclRegisterButton(GetScaledXPosition(440), GetScaledYPosition(450), 200, 15, "Connecting to GATEWAY...", "", "loading_text");
	EclRegisterButtonCallbacks("loading_text", text_draw, nullptr, nullptr, nullptr);
	RsArchiveFileClose(filePath, file);

	delete[] filePath;
}

void LoadingInterface::Remove()
{
	if (!IsVisible())
		return;

	EclRemoveButton("loading_background");
	EclRemoveButton("loading_text");
}

bool LoadingInterface::IsVisible()
{
	return EclGetButton("loading_background") != nullptr;
}

MainMenuScreenCode LoadingInterface::ScreenID()
{
	return MainMenuScreenCode::Loading;
}
