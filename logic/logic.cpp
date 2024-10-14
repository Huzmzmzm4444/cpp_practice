#include <stdexcept>
#include <iostream>

#include "Header2.h"


namespace {

    void check_loop(const logic::Element& loop, const logic::Element& elem) {
        if (&loop == &elem)
            throw std::runtime_error{ "loop detected in connections of logic elements" };

        for (const auto& out : loop.outputs())
            check_loop(out.get(), elem); 
    }

} // namespace

namespace logic {

    Input::Input(Element& elem, SignalState st)
        : m_elem{ elem }, m_inv{ st == SignalState::inverted } {}

    bool Input::signal() const {
        bool res = m_elem.get().signal(); 
        return m_inv ? !res : res;
    }

    SignalState Input::state() const {
        return m_inv ? SignalState::inverted : SignalState::direct;
    }

    Element::Element(Operation op, SignalState out)
        : m_type{ ElementType{int(op)} }, m_out_inv{ out == SignalState::inverted } {}

    Element::Element(SourceState st)
        : m_type{ ElementType::source }, m_src_on{ st == SourceState::on } {}

    bool Element::signal() const {
        bool s = calc(); 
        return m_out_inv ? !s : s;
    }

    SignalState Element::state() const {
        return m_out_inv ? SignalState::inverted : SignalState::direct;
    }

    bool Element::calc() const {
        switch (m_type) {
        case ElementType::source:
            return m_src_on;

        case ElementType::and_op:
            for (const auto& in : inputs()) {
                if (!in.signal())
                    return false;
            }
            return true;

        case ElementType::or_op:
            for (const auto& in : inputs()) {
                if (in.signal())
                    return true;
            }
            return false;
        }
        throw std::logic_error{ "unknown element type" };
    }

    void Element::set(SourceState st) {
        if (m_type != ElementType::source)
            throw std::runtime_error{ "can't set state for non-source element" };

        m_src_on = (st == SourceState::on);
    }

    Element& operator>>(Element& lhs, Input rhs) {
        auto op = rhs.element().m_type;

        if (op == ElementType::source)
            throw std::runtime_error{ "inputs for source elements not allowed" };

        check_loop(rhs.element(), lhs);

        
        rhs.element().m_inputs.push_back(Input{ lhs, rhs.state() });
        lhs.m_outputs.push_back(std::ref(rhs.element())); 

        return rhs.element(); 
    }

    Element& operator>>(Element& lhs, Element& rhs) {
        return lhs >> Input{ rhs, SignalState::direct };
    }

} // namespace logic


int main() {
    using logic::Element;
    using logic::SourceState;
    using logic::Operation;
    using logic::SignalState;


    Element src1{ SourceState::on };
    Element src2{ SourceState::off };
    Element and1{ Operation::and_op };
    Element and2{ Operation::and_op, SignalState::inverted };
    Element or1{ Operation::or_op };
    Element src3{ SourceState::off };
    Element src4{ SourceState::off };
    

 
    src1 >> and1; 
    src2 >> and1; 
    and1 >> and2; 
    src1 >> or1; 
    src2 >> or1; 

    src3 >> and2; 
    src4 >> and2;

    
    std::cout << "src1: " << src1.signal()
        << ", src2: " << src2.signal()
        << ", src3: " << src3.signal()
        << ", src4: " << src4.signal()
        << ", and1: " << and1.signal()
        << ", and2: " << and2.signal()
        << ", or1: " << or1.signal() << std::endl;

    // Change the state of src3 and src4
    src3.set(SourceState::on);
    src4.set(SourceState::on);

    std::cout << "After setting src3 and src4 on:" << std::endl;
    std::cout << "src1: " << src1.signal()
        << ", src2: " << src2.signal()
        << ", src3: " << src3.signal()
        << ", src4: " << src4.signal()
        << ", and1: " << and1.signal()
        << ", and2: " << and2.signal()
        << ", or1: " << or1.signal() << std::endl;

    return 0;
}

