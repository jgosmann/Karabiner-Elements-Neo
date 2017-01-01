#pragma once

#include "boost_defs.hpp"

#include "event_dispatcher_manager.hpp"
#include "gcd_utility.hpp"
#include "logger.hpp"
#include "manipulator.hpp"
#include "modifier_flag_manager.hpp"
#include "pointing_button_manager.hpp"
#include "system_preferences.hpp"
#include "types.hpp"
#include "virtual_hid_device_client.hpp"
#include <IOKit/hidsystem/ev_keymap.h>
#include <boost/optional.hpp>
#include <list>
#include <thread>
#include <unordered_map>

namespace manipulator {
class event_manipulator final {
public:
  event_manipulator(const event_manipulator&) = delete;

  event_manipulator(void) : virtual_hid_device_client_(logger::get_logger(),
                                                       std::bind(&event_manipulator::virtual_hid_device_client_connected_callback, this, std::placeholders::_1)),
                            event_dispatcher_manager_(),
                            key_repeat_manager_(*this) {
                                
                                //=== takahasix ============================================
                                //=== takahasix ============================================
                                //=== takahasix ============================================
                                // For user, rewrite this config, if you need.

                                // navigation keys
                                ck2ck_[ComplexKey(false, true, false, false, false, false, false, krbn::key_code(7))] =
                                    ComplexKey(false, false, false, false, false, false, false, krbn::key_code::down_arrow);
                                ck2ck_[ComplexKey(false, true, false, false, false, false, false, krbn::key_code(8))] =
                                    ComplexKey(false, false, false, false, false, false, false, krbn::key_code::up_arrow);
                                ck2ck_[ComplexKey(false, true, false, false, false, false, false, krbn::key_code(22))] =
                                    ComplexKey(false, false, false, false, false, false, false, krbn::key_code::left_arrow);
                                ck2ck_[ComplexKey(false, true, false, false, false, false, false, krbn::key_code(9))] =
                                    ComplexKey(false, false, false, false, false, false, false, krbn::key_code::right_arrow);
                                ck2ck_[ComplexKey(false, true, false, false, false, false, false, krbn::key_code(4))] =
                                    ComplexKey(false, false, false, false, false, false, false, krbn::key_code::home);
                                ck2ck_[ComplexKey(false, true, false, false, false, false, false, krbn::key_code(10))] =
                                    ComplexKey(false, false, false, false, false, false, false, krbn::key_code::end);
                                ck2ck_[ComplexKey(false, true, false, false, false, false, false, krbn::key_code(20))] =
                                    ComplexKey(false, false, false, false, false, false, false, krbn::key_code::page_up);
                                ck2ck_[ComplexKey(false, true, false, false, false, false, false, krbn::key_code(23))] =
                                    ComplexKey(false, false, false, false, false, false, false, krbn::key_code::page_down);

                                // escape
                                ck2ck_[ComplexKey(false, true, false, false, false, false, false, krbn::key_code(29))] =
                                    ComplexKey(false, false, false, false, false, false, false, krbn::key_code(41));

                                // deletion
                                ck2ck_[ComplexKey(false, true, false, false, false, false, false, krbn::key_code(26))] =
                                    ComplexKey(false, false, false, false, false, false, false, krbn::key_code(42));
                                ck2ck_[ComplexKey(false, true, false, false, false, false, false, krbn::key_code(21))] =
                                    ComplexKey(false, false, false, false, false, false, false, krbn::key_code::delete_forward);

                                // num pad
                                ck2ck_[ComplexKey(false, true, false, false, false, false, false, krbn::key_code(16))] =
                                    ComplexKey(false, false, false, false, false, false, false, krbn::key_code(89));
                                ck2ck_[ComplexKey(false, true, false, false, false, false, false, krbn::key_code(54))] =
                                    ComplexKey(false, false, false, false, false, false, false, krbn::key_code(90));
                                ck2ck_[ComplexKey(false, true, false, false, false, false, false, krbn::key_code(55))] =
                                    ComplexKey(false, false, false, false, false, false, false, krbn::key_code(91));
                                ck2ck_[ComplexKey(false, true, false, false, false, false, false, krbn::key_code(13))] =
                                    ComplexKey(false, false, false, false, false, false, false, krbn::key_code(92));
                                ck2ck_[ComplexKey(false, true, false, false, false, false, false, krbn::key_code(14))] =
                                    ComplexKey(false, false, false, false, false, false, false, krbn::key_code(93));
                                ck2ck_[ComplexKey(false, true, false, false, false, false, false, krbn::key_code(15))] =
                                    ComplexKey(false, false, false, false, false, false, false, krbn::key_code(94));
                                ck2ck_[ComplexKey(false, true, false, false, false, false, false, krbn::key_code(24))] =
                                    ComplexKey(false, false, false, false, false, false, false, krbn::key_code(95));
                                ck2ck_[ComplexKey(false, true, false, false, false, false, false, krbn::key_code(12))] =
                                    ComplexKey(false, false, false, false, false, false, false, krbn::key_code(96));
                                ck2ck_[ComplexKey(false, true, false, false, false, false, false, krbn::key_code(18))] =
                                    ComplexKey(false, false, false, false, false, false, false, krbn::key_code(97));
                                ck2ck_[ComplexKey(false, true, false, false, false, false, false, krbn::key_code(44))] =
                                    ComplexKey(false, false, false, false, false, false, false, krbn::key_code(98));
                                // divide
                                ck2ck_[ComplexKey(false, true, false, false, false, false, false, krbn::key_code(38))] =
                                    ComplexKey(false, false, false, false, false, false, false, krbn::key_code(103));
                                // multiply
                                ck2ck_[ComplexKey(false, true, false, false, false, false, false, krbn::key_code(39))] =
                                    ComplexKey(false, false, false, false, false, false, false, krbn::key_code(84));
                                // subtract
                                ck2ck_[ComplexKey(false, true, false, false, false, false, false, krbn::key_code(45))] =
                                    ComplexKey(false, false, false, false, false, false, false, krbn::key_code(85));
                                // add
                                ck2ck_[ComplexKey(false, true, false, false, false, false, false, krbn::key_code(19))] =
                                    ComplexKey(false, false, false, false, false, false, false, krbn::key_code(86));
                                // enter
                                ck2ck_[ComplexKey(false, true, false, false, false, false, false, krbn::key_code(25))] =
                                    ComplexKey(false, false, false, false, false, false, false, krbn::key_code(40));
                                // decimal point
                                ck2ck_[ComplexKey(false, true, false, false, false, false, false, krbn::key_code(52))] =
                                    ComplexKey(false, false, false, false, false, false, false, krbn::key_code(55));
                                // comma
                                ck2ck_[ComplexKey(false, true, false, false, false, false, false, krbn::key_code(51))] =
                                    ComplexKey(false, false, false, false, false, false, false, krbn::key_code(54));
                                // colon
                                ck2ck_[ComplexKey(false, true, false, false, false, false, false, krbn::key_code(17))] =
                                    ComplexKey(false, false, true, false, false, false, false, krbn::key_code(51));
                                // semicolon
                                ck2ck_[ComplexKey(false, true, false, false, false, false, false, krbn::key_code(56))] =
                                    ComplexKey(false, false, true, false, false, false, false, krbn::key_code(56));

                                // tab
                                ck2ck_[ComplexKey(false, true, false, false, false, false, false, krbn::key_code(27))] =
                                    ComplexKey(false, false, false, false, false, false, false, krbn::key_code(43));
                                ck2ck_[ComplexKey(false, true, false, false, false, false, false, krbn::key_code(37))] =
                                    ComplexKey(false, false, false, false, false, false, false, krbn::key_code(43));

                                // undo
                                ck2ck_[ComplexKey(false, true, false, false, false, false, false, krbn::key_code(35))] =
                                    ComplexKey(false, false, true, true, false, false, false, krbn::key_code(29));

                                //=== takahasix ============================================
                                //=== takahasix ============================================
                                //=== takahasix ============================================
                                
  }

  ~event_manipulator(void) {
  }

  enum class ready_state {
    ready,
    virtual_hid_device_client_is_not_ready,
    event_dispatcher_manager_is_not_ready,
  };

  ready_state is_ready(void) {
    if (!virtual_hid_device_client_.is_connected()) {
      return ready_state::virtual_hid_device_client_is_not_ready;
    }
    if (!event_dispatcher_manager_.is_connected()) {
      return ready_state::event_dispatcher_manager_is_not_ready;
    }
    return ready_state::ready;
  }

  void reset(void) {
    key_repeat_manager_.stop();

    manipulated_keys_.clear();
    manipulated_fn_keys_.clear();

    modifier_flag_manager_.reset();
    modifier_flag_manager_.unlock();

    virtual_hid_keyboard_pressed_keys_.clear();

    event_dispatcher_manager_.set_caps_lock_state(false);

    pointing_button_manager_.reset();

    // Do not call terminate_virtual_hid_keyboard
    virtual_hid_device_client_.terminate_virtual_hid_pointing();
  }

  void reset_modifier_flag_state(void) {
    modifier_flag_manager_.reset();
    // Do not call modifier_flag_manager_.unlock() here.
  }

  void reset_pointing_button_state(void) {
    auto bits = pointing_button_manager_.get_hid_report_bits();
    pointing_button_manager_.reset();
    if (bits) {
      virtual_hid_device_client_.reset_virtual_hid_pointing();
    }
  }

  void relaunch_event_dispatcher(void) {
    event_dispatcher_manager_.relaunch();
  }

  void set_system_preferences_values(const system_preferences::values& values) {
    std::lock_guard<std::mutex> guard(system_preferences_values_mutex_);

    system_preferences_values_ = values;
  }

  void clear_simple_modifications(void) {
    simple_modifications_.clear();
  }

  void add_simple_modification(krbn::key_code from_key_code, krbn::key_code to_key_code) {
    simple_modifications_.add(from_key_code, to_key_code);
  }

  void clear_fn_function_keys(void) {
    fn_function_keys_.clear();
  }

  void add_fn_function_key(krbn::key_code from_key_code, krbn::key_code to_key_code) {
    fn_function_keys_.add(from_key_code, to_key_code);
  }

  void create_event_dispatcher_client(void) {
    event_dispatcher_manager_.create_event_dispatcher_client();
  }

  void initialize_virtual_hid_pointing(void) {
    virtual_hid_device_client_.initialize_virtual_hid_pointing();
  }

  void terminate_virtual_hid_pointing(void) {
    virtual_hid_device_client_.terminate_virtual_hid_pointing();
  }

  void set_caps_lock_state(bool state) {
    modifier_flag_manager_.manipulate(krbn::modifier_flag::caps_lock,
                                      state ? modifier_flag_manager::operation::lock : modifier_flag_manager::operation::unlock);

    // Do not call event_dispatcher_manager_.set_caps_lock_state here.
    //
    // This method should be called in event_tap_manager_.caps_lock_state_changed_callback.
    // Thus, the caps lock state in IOHIDSystem is already changed.
  }

  void handle_keyboard_event(device_registry_entry_id device_registry_entry_id,
                             krbn::key_code from_key_code,
                             krbn::keyboard_type keyboard_type,
                             bool pressed) {
    krbn::key_code to_key_code = from_key_code;

    // ----------------------------------------
    // modify keys
    if (!pressed) {
      if (auto key_code = manipulated_keys_.find(device_registry_entry_id, from_key_code)) {
        manipulated_keys_.remove(device_registry_entry_id, from_key_code);
        to_key_code = *key_code;
      }
    } else {
      if (auto key_code = simple_modifications_.get(from_key_code)) {
        manipulated_keys_.add(device_registry_entry_id, from_key_code, *key_code);
        to_key_code = *key_code;
      }
    }

    // ----------------------------------------
    // modify fn+arrow, function keys
    if (!pressed) {
      if (auto key_code = manipulated_fn_keys_.find(device_registry_entry_id, to_key_code)) {
        manipulated_fn_keys_.remove(device_registry_entry_id, to_key_code);
        to_key_code = *key_code;
      }
    } else {
      boost::optional<krbn::key_code> key_code;

      if (modifier_flag_manager_.pressed(krbn::modifier_flag::fn)) {
        switch (to_key_code) {
        case krbn::key_code::return_or_enter:
          key_code = krbn::key_code::keypad_enter;
          break;
        case krbn::key_code::delete_or_backspace:
          key_code = krbn::key_code::delete_forward;
          break;
        case krbn::key_code::right_arrow:
          key_code = krbn::key_code::end;
          break;
        case krbn::key_code::left_arrow:
          key_code = krbn::key_code::home;
          break;
        case krbn::key_code::down_arrow:
          key_code = krbn::key_code::page_down;
          break;
        case krbn::key_code::up_arrow:
          key_code = krbn::key_code::page_up;
          break;
        default:
          break;
        }
      }

      // f1-f12
      {
        auto key_code_value = static_cast<uint32_t>(to_key_code);
        if (kHIDUsage_KeyboardF1 <= key_code_value && key_code_value <= kHIDUsage_KeyboardF12) {
          bool keyboard_fn_state = false;
          {
            std::lock_guard<std::mutex> guard(system_preferences_values_mutex_);
            keyboard_fn_state = system_preferences_values_.get_keyboard_fn_state();
          }

          bool fn_pressed = modifier_flag_manager_.pressed(krbn::modifier_flag::fn);

          if ((fn_pressed && keyboard_fn_state) ||
              (!fn_pressed && !keyboard_fn_state)) {
            // change f1-f12 keys to media controls
            if (auto k = fn_function_keys_.get(to_key_code)) {
              key_code = *k;
            }
          }
        }
      }

      if (key_code) {
        manipulated_fn_keys_.add(device_registry_entry_id, to_key_code, *key_code);
        to_key_code = *key_code;
      }
    }
      
      
      //=== takahasix ============================================
      //=== takahasix ============================================
      //=== takahasix ============================================
      if(shikakari_){
          
          // Stop key_repeat_manager.
          key_repeat_manager_.stop();
          
          ComplexKey src = shikakariDst_;
          ComplexKey dst = shikakariSrc_;
          
          // Clear src's key.
          post_key(src.key_, src.key_, keyboard_type, false, false);
          
          set_modifiers(src, keyboard_type, false);
          set_modifiers(dst, keyboard_type, true);
          
          shikakari_ = false;
      }
      
      
      if(pressed==true && krbn::types::get_modifier_flag(to_key_code)==krbn::modifier_flag::zero){
          const bool cmdLeft = modifier_flag_manager_.pressed(krbn::modifier_flag::left_command);
          const bool cmdRight = modifier_flag_manager_.pressed(krbn::modifier_flag::right_command);
          const bool opt = modifier_flag_manager_.pressed(krbn::modifier_flag::left_option) || modifier_flag_manager_.pressed(krbn::modifier_flag::right_option);
          const bool shiftLeft = modifier_flag_manager_.pressed(krbn::modifier_flag::left_shift);
          const bool shiftRight = modifier_flag_manager_.pressed(krbn::modifier_flag::right_shift);
          const bool ctrl = modifier_flag_manager_.pressed(krbn::modifier_flag::left_control) || modifier_flag_manager_.pressed(krbn::modifier_flag::right_control);
          const bool fn = modifier_flag_manager_.pressed(krbn::modifier_flag::fn);
          ComplexKey src(cmdLeft, cmdRight, opt, shiftLeft, shiftRight, ctrl, fn, to_key_code);
          if (ck2ck_.count(src) > 0) {
              ComplexKey dst = ck2ck_.at(src);
              set_modifiers(src, keyboard_type, false);
              set_modifiers(dst, keyboard_type, true);

              // Replace to_key_code with dst's key.
              to_key_code = dst.key_;

              shikakariSrc_ = src;
              shikakariDst_ = dst;
              shikakari_ = true;
          }
      }
      
      
      //=== takahasix ============================================
      //=== takahasix ============================================
      //=== takahasix ============================================


    // ----------------------------------------
    // Post input events to karabiner_event_dispatcher

    if (to_key_code == krbn::key_code::caps_lock) {
      if (auto hid_system_key = krbn::types::get_hid_system_key(to_key_code)) {
        if (pressed) {
          virtual_hid_keyboard_pressed_keys_.add(*hid_system_key);
          key_repeat_manager_.stop();
        } else {
          virtual_hid_keyboard_pressed_keys_.remove(*hid_system_key);
        }

        pqrs::karabiner_virtual_hid_device::hid_report::keyboard_input report;
        report.modifiers = modifier_flag_manager_.get_hid_report_bits();
        virtual_hid_keyboard_pressed_keys_.set_report_keys(report);
        virtual_hid_device_client_.post_keyboard_input_report(report);
      }
      return;
    }

    if (post_modifier_flag_event(to_key_code, keyboard_type, pressed)) {
      key_repeat_manager_.stop();
      return;
    }

    post_key(from_key_code, to_key_code, keyboard_type, pressed, false);

    // set key repeat
    long initial_key_repeat_milliseconds = 0;
    long key_repeat_milliseconds = 0;
    {
      std::lock_guard<std::mutex> guard(system_preferences_values_mutex_);
      initial_key_repeat_milliseconds = system_preferences_values_.get_initial_key_repeat_milliseconds();
      key_repeat_milliseconds = system_preferences_values_.get_key_repeat_milliseconds();
    }

    key_repeat_manager_.start(from_key_code, to_key_code, keyboard_type, pressed,
                              initial_key_repeat_milliseconds, key_repeat_milliseconds);
  }

  void handle_pointing_event(device_registry_entry_id device_registry_entry_id,
                             krbn::pointing_event pointing_event,
                             boost::optional<krbn::pointing_button> pointing_button,
                             CFIndex integer_value) {
    pqrs::karabiner_virtual_hid_device::hid_report::pointing_input report;

    switch (pointing_event) {
    case krbn::pointing_event::button:
      if (pointing_button && *pointing_button != krbn::pointing_button::zero) {
        pointing_button_manager_.manipulate(*pointing_button,
                                            integer_value ? pointing_button_manager::operation::increase : pointing_button_manager::operation::decrease);
      }
      break;

    case krbn::pointing_event::x:
      report.x = integer_value;
      break;

    case krbn::pointing_event::y:
      report.y = integer_value;
      break;

    case krbn::pointing_event::vertical_wheel:
      report.vertical_wheel = integer_value;
      break;

    case krbn::pointing_event::horizontal_wheel:
      report.horizontal_wheel = integer_value;
      break;

    default:
      break;
    }

    auto bits = pointing_button_manager_.get_hid_report_bits();
    report.buttons[0] = (bits >> 0) & 0xff;
    report.buttons[1] = (bits >> 8) & 0xff;
    report.buttons[2] = (bits >> 16) & 0xff;
    report.buttons[3] = (bits >> 24) & 0xff;
    virtual_hid_device_client_.post_pointing_input_report(report);
  }

  void stop_key_repeat(void) {
    key_repeat_manager_.stop();
  }

private:
  class manipulated_keys final {
  public:
    manipulated_keys(const manipulated_keys&) = delete;

    manipulated_keys(void) {
    }

    void clear(void) {
      std::lock_guard<std::mutex> guard(mutex_);

      manipulated_keys_.clear();
    }

    void add(device_registry_entry_id device_registry_entry_id,
             krbn::key_code from_key_code,
             krbn::key_code to_key_code) {
      std::lock_guard<std::mutex> guard(mutex_);

      manipulated_keys_.push_back(manipulated_key(device_registry_entry_id, from_key_code, to_key_code));
    }

    boost::optional<krbn::key_code> find(device_registry_entry_id device_registry_entry_id,
                                         krbn::key_code from_key_code) {
      std::lock_guard<std::mutex> guard(mutex_);

      for (const auto& v : manipulated_keys_) {
        if (v.get_device_registry_entry_id() == device_registry_entry_id &&
            v.get_from_key_code() == from_key_code) {
          return v.get_to_key_code();
        }
      }
      return boost::none;
    }

    void remove(device_registry_entry_id device_registry_entry_id,
                krbn::key_code from_key_code) {
      std::lock_guard<std::mutex> guard(mutex_);

      manipulated_keys_.remove_if([&](const manipulated_key& v) {
        return v.get_device_registry_entry_id() == device_registry_entry_id &&
               v.get_from_key_code() == from_key_code;
      });
    }

  private:
    class manipulated_key final {
    public:
      manipulated_key(device_registry_entry_id device_registry_entry_id,
                      krbn::key_code from_key_code,
                      krbn::key_code to_key_code) : device_registry_entry_id_(device_registry_entry_id),
                                                    from_key_code_(from_key_code),
                                                    to_key_code_(to_key_code) {
      }

      device_registry_entry_id get_device_registry_entry_id(void) const { return device_registry_entry_id_; }
      krbn::key_code get_from_key_code(void) const { return from_key_code_; }
      krbn::key_code get_to_key_code(void) const { return to_key_code_; }

    private:
      device_registry_entry_id device_registry_entry_id_;
      krbn::key_code from_key_code_;
      krbn::key_code to_key_code_;
    };

    std::list<manipulated_key> manipulated_keys_;
    std::mutex mutex_;
  };

  class virtual_hid_keyboard_pressed_keys final {
  public:
    virtual_hid_keyboard_pressed_keys(const virtual_hid_keyboard_pressed_keys&) = delete;

    virtual_hid_keyboard_pressed_keys(void) {
    }

    void clear(void) {
      std::lock_guard<std::mutex> guard(mutex_);

      keys_.clear();
    }

    void add(uint8_t hid_system_key) {
      std::lock_guard<std::mutex> guard(mutex_);

      if (std::find(keys_.begin(), keys_.end(), hid_system_key) == keys_.end()) {
        keys_.push_back(hid_system_key);
      }
    }

    void remove(uint8_t hid_system_key) {
      std::lock_guard<std::mutex> guard(mutex_);

      keys_.remove(hid_system_key);
    }

    void set_report_keys(pqrs::karabiner_virtual_hid_device::hid_report::keyboard_input& report) {
      size_t i = 0;
      for (const auto& key : keys_) {
        if (i >= sizeof(report.keys) / sizeof(report.keys[0])) {
          break;
        }
        report.keys[i] = key;
        ++i;
      }
    }

  private:
    std::list<uint8_t> keys_;
    std::mutex mutex_;
  };

  class simple_modifications final {
  public:
    simple_modifications(const simple_modifications&) = delete;

    simple_modifications(void) {
    }

    void clear(void) {
      std::lock_guard<std::mutex> guard(mutex_);

      map_.clear();
    }

    void add(krbn::key_code from_key_code, krbn::key_code to_key_code) {
      std::lock_guard<std::mutex> guard(mutex_);

      map_[from_key_code] = to_key_code;
    }

    boost::optional<krbn::key_code> get(krbn::key_code from_key_code) {
      std::lock_guard<std::mutex> guard(mutex_);

      auto it = map_.find(from_key_code);
      if (it != map_.end()) {
        return it->second;
      }

      return boost::none;
    }

  private:
    std::unordered_map<krbn::key_code, krbn::key_code> map_;
    std::mutex mutex_;
  };

  class key_repeat_manager final {
  public:
    key_repeat_manager(const key_repeat_manager&) = delete;

    key_repeat_manager(event_manipulator& event_manipulator) : event_manipulator_(event_manipulator),
                                                               timer_(nullptr) {
    }

    ~key_repeat_manager(void) {
      stop();
    }

    void start(krbn::key_code from_key_code, krbn::key_code to_key_code,
               krbn::keyboard_type keyboard_type, bool pressed,
               long initial_key_repeat_milliseconds, long key_repeat_milliseconds) {
      // stop key repeat before post key.
      if (pressed) {
        stop();
      } else {
        if (from_key_code_ && *from_key_code_ == from_key_code) {
          stop();
        }
      }

      // set key repeat
      if (pressed) {
        if (to_key_code == krbn::key_code::mute ||
            to_key_code == krbn::key_code::vk_consumer_play) {
          return;
        }

        timer_ = std::make_unique<gcd_utility::main_queue_timer>(
            DISPATCH_TIMER_STRICT,
            dispatch_time(DISPATCH_TIME_NOW, initial_key_repeat_milliseconds * NSEC_PER_MSEC),
            key_repeat_milliseconds * NSEC_PER_MSEC,
            0,
            ^{
              event_manipulator_.post_key(from_key_code, to_key_code, keyboard_type, pressed, true);
            });

        from_key_code_ = from_key_code;
      }
    }

    void stop(void) {
      timer_ = nullptr;
    }

  private:
    event_manipulator& event_manipulator_;

    std::unique_ptr<gcd_utility::main_queue_timer> timer_;

    boost::optional<krbn::key_code> from_key_code_;
  };

  void virtual_hid_device_client_connected_callback(virtual_hid_device_client& virtual_hid_device_client) {
    virtual_hid_device_client.initialize_virtual_hid_keyboard();
  }

  bool post_modifier_flag_event(krbn::key_code key_code, krbn::keyboard_type keyboard_type, bool pressed) {
    auto operation = pressed ? manipulator::modifier_flag_manager::operation::increase : manipulator::modifier_flag_manager::operation::decrease;

    auto modifier_flag = krbn::types::get_modifier_flag(key_code);
    if (modifier_flag != krbn::modifier_flag::zero) {
      modifier_flag_manager_.manipulate(modifier_flag, operation);

      if (modifier_flag == krbn::modifier_flag::fn) {
        auto flags = modifier_flag_manager_.get_io_option_bits(key_code);
        event_dispatcher_manager_.post_modifier_flags(key_code, flags, keyboard_type);
      } else {
        pqrs::karabiner_virtual_hid_device::hid_report::keyboard_input report;
        report.modifiers = modifier_flag_manager_.get_hid_report_bits();
        virtual_hid_keyboard_pressed_keys_.set_report_keys(report);
        virtual_hid_device_client_.post_keyboard_input_report(report);
      }

      return true;
    }

    return false;
  }

  void post_key(krbn::key_code from_key_code, krbn::key_code to_key_code, krbn::keyboard_type keyboard_type, bool pressed, bool repeat) {
    auto hid_system_key = krbn::types::get_hid_system_key(to_key_code);
    auto hid_system_aux_control_button = krbn::types::get_hid_system_aux_control_button(to_key_code);
    if (hid_system_key || hid_system_aux_control_button) {
      auto event_type = pressed ? krbn::event_type::key_down : krbn::event_type::key_up;
      auto flags = modifier_flag_manager_.get_io_option_bits(to_key_code);
      event_dispatcher_manager_.post_key(to_key_code, event_type, flags, keyboard_type, repeat);
    }
  }

  virtual_hid_device_client virtual_hid_device_client_;
  virtual_hid_keyboard_pressed_keys virtual_hid_keyboard_pressed_keys_;
  event_dispatcher_manager event_dispatcher_manager_;
  modifier_flag_manager modifier_flag_manager_;
  pointing_button_manager pointing_button_manager_;
  key_repeat_manager key_repeat_manager_;

  system_preferences::values system_preferences_values_;
  std::mutex system_preferences_values_mutex_;

  simple_modifications simple_modifications_;
  simple_modifications fn_function_keys_;

  manipulated_keys manipulated_keys_;
  manipulated_keys manipulated_fn_keys_;
    
    //=== takahasix ============================================
    //=== takahasix ============================================
    //=== takahasix ============================================
    class ComplexKey final {
    public:
        ComplexKey(){
        }
        
        ComplexKey(bool commandLeft, bool commandRight, bool option, bool shiftLeft, bool shiftRight, bool control, bool fn, krbn::key_code key){
            commandRight_ = commandRight;
            commandLeft_ = commandLeft;
            option_ = option;
            shiftLeft_ = shiftLeft;
            shiftRight_ = shiftRight;
            control_ = control;
            fn_ = fn;
            key_ = key;
        }
        
        bool operator<(const ComplexKey &ck) const {
            if(key_ < ck.key_){
                return true;
            }else if(key_ > ck.key_){
                return false;
            }else{
                if(commandLeft_ < ck.commandLeft_){
                    return true;
                }else if(commandLeft_ > ck.commandLeft_){
                    return false;
                }else{
                    if(commandRight_ < ck.commandRight_){
                        return true;
                    }else if(commandRight_ > ck.commandRight_){
                        return false;
                    }else{
                        if(option_ < ck.option_){
                            return true;
                        }else if(option_ > ck.option_){
                            return false;
                        }else{
                            if(shiftLeft_ < ck.shiftLeft_){
                                return true;
                            }else if(shiftLeft_ > ck.shiftLeft_){
                                return false;
                            }else{
                                if(shiftRight_ < ck.shiftRight_){
                                    return true;
                                }else if(shiftRight_ > ck.shiftRight_){
                                    return false;
                                }else{
                                    if(control_ < ck.control_){
                                        return true;
                                    }else if(control_ > ck.control_){
                                        return false;
                                    }else{
                                        if(fn_ < ck.fn_){
                                            return true;
                                        }else if(fn_ > ck.fn_){
                                            return false;
                                        }else{
                                            return false;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        
        bool operator==(const ComplexKey &ck) const {
            if(key_ != ck.key_)return false;
            if(commandLeft_ != ck.commandLeft_)return false;
            if(commandRight_ != ck.commandRight_)return false;
            if(option_ != ck.option_)return false;
            if(shiftLeft_ != ck.shiftLeft_)return false;
            if(shiftRight_ != ck.shiftRight_)return false;
            if(control_ != ck.control_)return false;
            if(fn_ != ck.fn_)return false;
            return true;
        }
        
        
        bool commandLeft_;
        bool commandRight_;
        bool option_;
        bool shiftLeft_;
        bool shiftRight_;
        bool control_;
        bool fn_;
        krbn::key_code key_;
    };
    
    
    std::map<ComplexKey, ComplexKey> ck2ck_;
    bool shikakari_ = false;
    ComplexKey shikakariSrc_;
    ComplexKey shikakariDst_;

  void set_modifiers(const ComplexKey &ck, krbn::keyboard_type keyboard_type, bool pressed) {
      if(ck.commandLeft_){
          post_modifier_flag_event(krbn::key_code(227), keyboard_type, pressed);
      }
      if(ck.commandRight_){
          post_modifier_flag_event(krbn::key_code(231), keyboard_type, pressed);
      }
      if(ck.option_){
          post_modifier_flag_event(krbn::key_code(226), keyboard_type, pressed);
      }
      if(ck.shiftLeft_){
          post_modifier_flag_event(krbn::key_code(225), keyboard_type, pressed);
      }
      if(ck.shiftRight_){
          post_modifier_flag_event(krbn::key_code(229), keyboard_type, pressed);
      }
      if(ck.control_){
          post_modifier_flag_event(krbn::key_code(224), keyboard_type, pressed);
      }
      if(ck.fn_){
          post_modifier_flag_event(krbn::key_code(4098), keyboard_type, pressed);
      }
  }
    //=== takahasix ============================================
    //=== takahasix ============================================
    //=== takahasix ============================================
    
};
}
