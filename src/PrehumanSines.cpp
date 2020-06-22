#include "plugin.hpp"


struct PrehumanSines : Module {
	enum ParamIds {
		FREQPARAM_PARAM,
		FREQPARAM_3_PARAM,
		FREQPARAM_7_0_PARAM,
		FREQPARAM_7_PARAM,
		FREQPARAM_6_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		NUM_INPUTS
	};
	enum OutputIds {
		PATH89_OUTPUT,
		PATH89_2_OUTPUT,
		PATH89_9_OUTPUT,
		PATH89_1_OUTPUT,
		PATH89_27_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	PrehumanSines() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(FREQPARAM_PARAM, 0.f, 1.f, 0.f, "");
		configParam(FREQPARAM_3_PARAM, 0.f, 1.f, 0.f, "");
		configParam(FREQPARAM_7_0_PARAM, 0.f, 1.f, 0.f, "");
		configParam(FREQPARAM_7_PARAM, 0.f, 1.f, 0.f, "");
		configParam(FREQPARAM_6_PARAM, 0.f, 1.f, 0.f, "");
	}

	void process(const ProcessArgs &args) override {
	}
};


struct PrehumanSinesWidget : ModuleWidget {
	PrehumanSinesWidget(PrehumanSines *module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/PrehumanSines.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(21.82, 24.755)), module, PrehumanSines::FREQPARAM_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(64.98, 24.755)), module, PrehumanSines::FREQPARAM_3_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(43.4, 58.208)), module, PrehumanSines::FREQPARAM_7_0_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(21.82, 91.617)), module, PrehumanSines::FREQPARAM_7_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(64.98, 91.617)), module, PrehumanSines::FREQPARAM_6_PARAM));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(14.406, 118.878)), module, PrehumanSines::PATH89_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(28.903, 119.146)), module, PrehumanSines::PATH89_2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(43.4, 119.413)), module, PrehumanSines::PATH89_9_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(57.897, 119.68)), module, PrehumanSines::PATH89_1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(72.394, 119.947)), module, PrehumanSines::PATH89_27_OUTPUT));
	}
};


Model *modelPrehumanSines = createModel<PrehumanSines, PrehumanSinesWidget>("PrehumanSines");