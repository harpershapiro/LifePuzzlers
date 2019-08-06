#include "plugin.hpp"


struct KingMod : Module {
	enum ParamIds {
		FREQ_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		AUDIO_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		AUDIO_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	float phase = 0.f;
	float outgain = 2.f;

	KingMod() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(FREQ_PARAM, 0.f, 1.f, 0.5f, "Frequency", " Hz", std::pow(2, 10.f), dsp::FREQ_C4 / std::pow(2, 5.f));
	}

	void process(const ProcessArgs &args) override {
		//get inputs and params
		Input &audioInput = inputs[AUDIO_INPUT];
		float input = audioInput.getVoltageSum();
		float freqParam = params[FREQ_PARAM].getValue() *10.f - 5.f;

		//calculate inner sinwave
		float freq = dsp::FREQ_C4 * std::pow(2.f,freqParam);

		phase+=freq*args.sampleTime;
		if(phase >= 0.5f)
			phase -=1.f;

		float modulator = std::sin(2.f * M_PI * phase);
		float carrier = clamp(modulator*input,0.f,1.f);

		//set outs
		Output &out = outputs[AUDIO_OUTPUT];
		out.setVoltage(outgain*carrier);

	}
};


struct KingModWidget : ModuleWidget {
	KingModWidget(KingMod *module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/KingMod.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(25.4, 37.241)), module, KingMod::AUDIO_INPUT));

		addParam(createParamCentered<RoundHugeBlackKnob>(mm2px(Vec(25.4, 70)), module, KingMod::FREQ_PARAM));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(25.4, 116.206)), module, KingMod::AUDIO_OUTPUT));
	}
};


Model *modelKingMod = createModel<KingMod, KingModWidget>("KingMod");