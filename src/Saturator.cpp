#include "plugin.hpp"


struct Saturator : Module {
	enum ParamIds {
		INGAIN_PARAM,
		OUTGAIN_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		PATH17_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		PATH19_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	float tanhLookup[101] = {-0.99627207622075, -0.9957739590782865, -0.9952094443176224, -0.9945697268229151, -0.9938448467680954, -0.993023541875534, -0.9920930818254032, -0.991039082964038, -0.9898453013591358, -0.988493402175018, -0.9869627033058325, -0.9852298912260578, -0.9832687071185617, -0.9810496015495929, -0.9785393563137075, -0.9757006726149112, -0.9724917255386386, -0.9688656858696789, -0.9647702118035602, -0.9601469150761827, -0.9549308086042282, -0.9490497460029758, -0.9424238674500077, -0.9349650714111251, -0.9265765378398192, -0.9171523356672744, -0.9065771557120564, -0.8947262194457066, -0.8814654240811349, -0.8666517947108041, -0.8501343239219393, -0.8317552872982551, -0.8113521278920903, -0.788760002064728, -0.7638150705607842, -0.7363585995164269, -0.7062419034803165, -0.6733321140484185, -0.6375186920428363, -0.5987205188492946, -0.5568933069002107, -0.5120369672627675, -0.46420247477786125, -0.41349769282645016, -0.3600915776188121, -0.3042161928867445, -0.24616604339791912, -0.1862943855962673, -0.12500639056040364, -0.06274930002740721, 0.0, 0.06274930002740721, 0.12500639056040364, 0.1862943855962673, 0.24616604339791912, 0.3042161928867445, 0.3600915776188121, 0.41349769282645016, 0.46420247477786125, 0.5120369672627675, 0.5568933069002107, 0.5987205188492946, 0.6375186920428363, 0.6733321140484185, 0.7062419034803165, 0.7363585995164269, 0.7638150705607842, 0.788760002064728, 0.8113521278920903, 0.8317552872982551, 0.8501343239219393, 0.8666517947108041, 0.8814654240811349, 0.8947262194457066, 0.9065771557120564, 0.9171523356672744, 0.9265765378398192, 0.9349650714111251, 0.9424238674500077, 0.9490497460029758, 0.9549308086042282, 0.9601469150761827, 0.9647702118035602, 0.9688656858696789, 0.9724917255386386, 0.9757006726149112, 0.9785393563137075, 0.9810496015495929, 0.9832687071185617, 0.9852298912260578, 0.9869627033058325, 0.988493402175018, 0.9898453013591358, 0.991039082964038, 0.9920930818254032, 0.993023541875534, 0.9938448467680954, 0.9945697268229151, 0.9952094443176224, 0.9957739590782865, 0.99627207622075};
	int maxInput=160;

	Saturator() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(INGAIN_PARAM,0.f,1.f,0.f,"IN GAIN","",0,10);
		configParam(OUTGAIN_PARAM,0.f,1.f,0.f,"OUT GAIN","",0,10);

	}

	void process(const ProcessArgs &args) override {
		Input audioinput = inputs[PATH17_INPUT];
		float input = audioinput.getVoltage();
		float inGainParam = params[INGAIN_PARAM].getValue();
		float outGainParam = params[OUTGAIN_PARAM].getValue();

		float inGain = pow(1.f + inGainParam, 5); //gain from drive
		float outGain = pow(1.f + outGainParam,5);
		input*=inGain;


		//want abnormal index to go from 0 to 120 for a bit of clipping
		int abnormalIndex = (input/maxInput)*80+80;
		int tanhindex = clamp(abnormalIndex,0,100);
		float tanhvalue = tanhLookup[tanhindex];
		//printf("GAIN: %f    INPUT: %f   AbINDEX: %i   SAT VALUE: %f \n",gain,input,abnormalIndex,tanhvalue);


		//int channels = audioinput.getChannels();
		//float in[16] = {};
		//audioinput.readVoltages(in);
		/**
		simd::float_4 v[4];
		for (int c = 0; c < channels; c += 4) {
			v[c / 4] = simd::float_4::load(in.getVoltages(c));
		}**/


		Output &out = outputs[PATH19_OUTPUT];
		out.setVoltage(outGain*tanhvalue);
		//out.setChannels(channels);
		//out.writeVoltages(in);
		/**
		for (int c = 0; c < channels; c += 4) {
			v[c / 4].store(out.getVoltages(c));
		}**/

	}
};


struct SaturatorWidget : ModuleWidget {
	SaturatorWidget(Saturator *module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Saturator.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(19.874, 26.241)), module, Saturator::PATH17_INPUT));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(26.727, 62.7)), module, Saturator::INGAIN_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(26.727, 82.7)), module, Saturator::OUTGAIN_PARAM));


		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(19.874, 74.617)), module, Saturator::PATH19_OUTPUT));
	}
};


Model *modelSaturator = createModel<Saturator, SaturatorWidget>("Saturator");