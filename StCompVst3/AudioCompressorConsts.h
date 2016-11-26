#pragma once


namespace Steinberg {
	namespace Vst {
		namespace StComp {

			enum ParameterIds {
				kThreshold = 0,
				kRatio,
				kAttack,
				kRelease,
				kOutput,
				kKnee,
				kReduction
			};

			class ConstantValues {
			public:
				inline static const double MaximumMakeupGain() { return 12.0; }
			};
		}
	}
}