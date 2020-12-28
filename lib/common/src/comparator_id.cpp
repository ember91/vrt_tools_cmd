#include <memory>

#include "vrt/vrt_types.h"
#include "vrt/vrt_util.h"

namespace vrt::common {

/**
 * Comparator to compare packets by Class and Stream ID.
 */
struct ComparatorId {
    /**
     * Compare packets by Class and Stream ID.
     *
     * \param a Packet 1.
     * \param b Packet 2.
     *
     * \return True if a is less than b.
     */
    bool operator()(const std::shared_ptr<vrt_packet>& a, const std::shared_ptr<vrt_packet>& b) const {
        if (!a->header.has.class_id && b->header.has.class_id) {
            return true;
        }
        if (a->header.has.class_id && !b->header.has.class_id) {
            return false;
        }
        // Either both or none has Class ID here
        if (a->header.has.class_id && b->header.has.class_id) {
            if (a->fields.class_id.oui < b->fields.class_id.oui) {
                return true;
            }
            if (a->fields.class_id.oui > b->fields.class_id.oui) {
                return false;
            }
            // OUI equal
            if (a->fields.class_id.information_class_code < b->fields.class_id.information_class_code) {
                return true;
            }
            if (a->fields.class_id.information_class_code > b->fields.class_id.information_class_code) {
                return false;
            }
            // OUI and Information class code equal
            if (a->fields.class_id.packet_class_code < b->fields.class_id.packet_class_code) {
                return true;
            }
            if (a->fields.class_id.packet_class_code > b->fields.class_id.packet_class_code) {
                return false;
            }
            // OUI, Information class code and Packet class code are equal
        }
        if (!vrt_has_stream_id(&a->header) && vrt_has_stream_id(&b->header)) {
            return true;
        }
        if (vrt_has_stream_id(&a->header) && !vrt_has_stream_id(&b->header)) {
            return false;
        }
        if (vrt_has_stream_id(&a->header) && vrt_has_stream_id(&b->header)) {
            if (a->fields.stream_id < b->fields.stream_id) {
                return true;
            }
            if (a->fields.stream_id > b->fields.stream_id) {
                return false;
            }
        }

        // Equality
        return false;
    }
};

}  // namespace vrt::common
