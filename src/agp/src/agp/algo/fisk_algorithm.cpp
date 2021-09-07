#include <agp/algo/fisk_algorithm.h>
#include <type_traits>
#include <agp/plot/agp_plot.h>

template <typename Iterator, typename Val>
void clear_all (Iterator it, Iterator end, Val v) {
    for (; it != end; ++it)
        it->set_data(v);
}


template <typename Arrangement>
typename Arrangement::Face_handle handle_halfedge_neighbour (typename Arrangement::Halfedge_handle &he) {
    auto neigh = he->twin()->face();
    if (neigh->is_unbounded() || neigh->data()) return neigh;
    auto vertex = he->next()->target();
    auto mirror_vertex = he->twin()->next()->target();
    mirror_vertex->set_data(vertex->data());
    return neigh;
}


template <typename Arrangement>
typename Arrangement::Face_handle finite_face (Arrangement &arrangement) {
    if (arrangement.number_of_faces() == arrangement.number_of_unbounded_faces())
        return typename Arrangement::Face_handle();
    for (auto fit = arrangement.faces_begin(); fit != arrangement.faces_end(); ++fit) {
        if (!fit->is_unbounded()) {
            return fit;
        }
    }
    return typename Arrangement::Face_handle();
}


template <typename Arrangement>
void colour_triangle (typename Arrangement::Face_handle &face) {
    std::vector<CGAL::Color> colours = { CGAL::BLUE, CGAL::RED, CGAL::GREEN };
    auto circ = face->outer_ccb(), curr = circ;
    int i = 0;
    do {
        curr->target()->set_data(colours[i]);
    } while (++curr != circ && ++i < 3);
    assert(curr == circ);
}


Extended_Arrangement_2 fisk_arrangement (const std::vector<Kernel::Point_2> &points) {
    auto triangulated = triangulate<Extended_Arrangement_2>(points);
    clear_all(triangulated.faces_begin(), triangulated.faces_end(), 0);

    auto first = finite_face(triangulated);
    colour_triangle<Extended_Arrangement_2>(first);

    std::queue<Extended_Arrangement_2::Face_handle> q;
    q.push(first);

    while (!q.empty()) {
        auto current = q.front();
        q.pop();

        auto circ = current->outer_ccb(), curr = circ;
        do {
            auto face = handle_halfedge_neighbour<Extended_Arrangement_2>(curr);
            if (face->is_unbounded() || face->data()) continue;
            q.push(face);
        } while (++curr != circ);

        current->set_data(1);
    }
    return triangulated;
}

std::vector<Kernel::Point_2> fisk_algorithm (const std::vector<Kernel::Point_2> &points) {
    auto arr = fisk_arrangement(points);
    std::vector<std::vector<Kernel::Point_2>> vertices = { std::vector<Kernel::Point_2>(), std::vector<Kernel::Point_2>(), std::vector<Kernel::Point_2>() };
    std::vector<CGAL::Color> colors = { CGAL::RED, CGAL::GREEN, CGAL::BLUE };
    for (auto vit = arr.vertices_begin(); vit != arr.vertices_end(); ++vit) {
        auto col = std::find(colors.begin(), colors.end(), vit->data()) - colors.begin();
        vertices[col].push_back(vit->point());
    }
    if (vertices[0].size() <= vertices[1].size() && vertices[0].size() <= vertices[2].size())
        return vertices[0];
    else if (vertices[1].size() <= vertices[2].size())
        return vertices[1];
    return vertices[2];
}
